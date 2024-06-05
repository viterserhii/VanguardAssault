#include "TankPawn.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
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
}

void ATankPawn::BeginPlay()
{
    Super::BeginPlay();
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
    }
    else
    {
        if (LeftDustEffect1->IsActive()) LeftDustEffect1->Deactivate();
        if (LeftDustEffect2->IsActive()) LeftDustEffect2->Deactivate();
        if (RightDustEffect1->IsActive()) RightDustEffect1->Deactivate();
        if (RightDustEffect2->IsActive()) RightDustEffect2->Deactivate();
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

    if (bCanFire && ProjectileClass)
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

        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ATankPawn::ResetFire, FireRate, false);
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
