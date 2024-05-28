#include "TankPawn.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

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
    if (!FMath::IsNearlyEqual(Value, CurrentAcceleration, 0.001f))
    {
        CurrentAcceleration = FMath::FInterpTo(CurrentAcceleration, Value, GetWorld()->DeltaTimeSeconds, 1.0f / AccelerationDuration);
    }

    FVector DeltaLocation = FVector(CurrentAcceleration * 1200.0f * GetWorld()->DeltaTimeSeconds, 0.f, 0.f);
    AddActorLocalOffset(DeltaLocation, true);
}


void ATankPawn::TurnRight(float Value)
{
    if (Value != 0.0f)
    {
        FRotator DeltaRotation = FRotator(0.f, Value * 30.0f * GetWorld()->DeltaTimeSeconds, 0.f);
        AddActorLocalRotation(DeltaRotation);
    }
}

void ATankPawn::Fire()
{
    if (bCanFire && ProjectileClass)
    {
        FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
        FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

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
