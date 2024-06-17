#include "TankPawn.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "MyGameMode.h"

ATankPawn::ATankPawn()
{
    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
    MovementComponent->UpdatedComponent = RootComponent;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.f; 

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    bUseControllerRotationYaw = false;

    LeftDustEffect1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftDustEffect1"));
    LeftDustEffect1->SetupAttachment(RootComponent);

    LeftDustEffect2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftDustEffect2"));
    LeftDustEffect2->SetupAttachment(RootComponent);

    RightDustEffect1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightDustEffect1"));
    RightDustEffect1->SetupAttachment(RootComponent);

    RightDustEffect2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightDustEffect2"));
    RightDustEffect2->SetupAttachment(RootComponent);

    LeftDustEffect1->bAutoActivate = false;
    LeftDustEffect2->bAutoActivate = false;
    RightDustEffect1->bAutoActivate = false;
    RightDustEffect2->bAutoActivate = false;

    EngineAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudioComponent"));
    EngineAudioComponent->SetupAttachment(RootComponent);
    EngineAudioComponent->bAutoActivate = true;
    EngineAudioComponent->SetSound(EngineSoundCue);

    TreadAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TreadAudioComponent"));
    TreadAudioComponent->SetupAttachment(RootComponent);
    TreadAudioComponent->bAutoActivate = false;

    MaxAmmo = 20;
    CurrentAmmo = MaxAmmo;
}

void ATankPawn::BeginPlay()
{
    Super::BeginPlay();

    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddDynamic(this, &ATankPawn::OnHealthChanged);

        OnHealthChanged(HealthComponent->GetCurrentHealth() / HealthComponent->GetMaxHealth());
    }

    AMyGameHUD* HUD = Cast<AMyGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (HUD)
    {
        HUD->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
    }
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ATankPawn::MoveForward);
    PlayerInputComponent->BindAxis("TurnRight", this, &ATankPawn::TurnRight);
}

void ATankPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AimTowardsMousePosition();
}

void ATankPawn::MoveForward(float Value)
{
    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode && !GameMode->bGameStarted)
    {
        return;
    }

    if (!FMath::IsNearlyEqual(Value, CurrentAcceleration, 0.001f))
    {
        CurrentAcceleration = FMath::FInterpTo(CurrentAcceleration, Value, GetWorld()->DeltaTimeSeconds, 1.0f / AccelerationDuration);
    }

    FVector DeltaLocation = FVector(CurrentAcceleration * 1200.0f * GetWorld()->DeltaTimeSeconds, 0.f, 0.f);
    AddActorLocalOffset(DeltaLocation, true);

    if (FMath::Abs(CurrentAcceleration) > 0.1f)
    {
        if (!LeftDustEffect1->IsActive()) LeftDustEffect1->Activate();
        if (!LeftDustEffect2->IsActive()) LeftDustEffect2->Activate();
        if (!RightDustEffect1->IsActive()) RightDustEffect1->Activate();
        if (!RightDustEffect2->IsActive()) RightDustEffect2->Activate();

        if (TreadSoundCue && !TreadAudioComponent->IsPlaying())
        {
            TreadAudioComponent->SetSound(TreadSoundCue);
            TreadAudioComponent->Play();
        }
    }
    else
    {
        if (LeftDustEffect1->IsActive()) LeftDustEffect1->Deactivate();
        if (LeftDustEffect2->IsActive()) LeftDustEffect2->Deactivate();
        if (RightDustEffect1->IsActive()) RightDustEffect1->Deactivate();
        if (RightDustEffect2->IsActive()) RightDustEffect2->Deactivate();

        if (TreadAudioComponent->IsPlaying())
        {
            TreadAudioComponent->Stop();
        }
    }
}

void ATankPawn::TurnRight(float Value)
{
    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode && !GameMode->bGameStarted)
    {
        return;
    }

    if (Value != 0.0f)
    {
        FRotator DeltaRotation = FRotator(0.f, Value * 30.0f * GetWorld()->DeltaTimeSeconds, 0.f);
        AddActorLocalRotation(DeltaRotation);
    }
}

void ATankPawn::Fire()
{
    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode && !GameMode->bGameStarted)
    {
        return;
    }

    if (bCanFire && ProjectileClass && CurrentAmmo > 0)
    {
        FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
        FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

        if (FireEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, SpawnLocation, SpawnRotation);
        }

        if (FireSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSoundCue, GetActorLocation());
        }

        if (ShootCameraShake)
        {
            GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ShootCameraShake);
        }

        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ATankPawn::ResetFire, FireRate, false);

        CurrentAmmo--;

        AMyGameHUD* HUD = Cast<AMyGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
        if (HUD)
        {
            HUD->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
        }
    }
}

void ATankPawn::ResetFire()
{
    bCanFire = true;
}

void ATankPawn::AimTowardsMousePosition()
{
    float MouseX, MouseY;
    APlayerController* PlayerController = Cast<APlayerController>(GetController());

    if (PlayerController && PlayerController->GetMousePosition(MouseX, MouseY))
    {
        FVector WorldLocation, WorldDirection;
        if (PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
        {
            FVector StartLocation = WorldLocation;
            FVector EndLocation = StartLocation + WorldDirection * 10000;

            FHitResult HitResult;
            if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
            {
                FVector HitLocation = HitResult.ImpactPoint;
                FVector Direction = HitLocation - TurretMesh->GetComponentLocation();
                Direction.Z = 0; 

                Direction = Direction.RotateAngleAxis(-90.0f, FVector(0.0f, 0.0f, 1.0f));

                RotateTurret(Direction);

                DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 32.0f, 12, FColor::White, false, -1.0f, 0, 2.0f);
            }
        }
    }
}

void ATankPawn::PlayEngineSound()
{
    EngineAudioComponent->SetSound(EngineSoundCue);
    EngineAudioComponent->Play();
}

void ATankPawn::OnHealthChanged(float HealthPercentage)
{
    AMyGameHUD* HUD = Cast<AMyGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (HUD)
    {
        HUD->UpdateHealthBar(HealthPercentage);
    }
}

void ATankPawn::AddAmmo(int32 AmmoAmount)
{
    CurrentAmmo = FMath::Clamp(CurrentAmmo + AmmoAmount, 0, MaxAmmo);

    AMyGameHUD* HUD = Cast<AMyGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (HUD)
    {
        HUD->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
    }
}