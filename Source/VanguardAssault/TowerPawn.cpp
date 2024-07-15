#include "TowerPawn.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameMode.h"
#include "TankPawn.h"

ATowerPawn::ATowerPawn()
{
    bReplicates = true;
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(RootComponent);
    DetectionSphere->SetSphereRadius(2000.0f);

    TeamColor = 2;
}

void ATowerPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ATowerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ATowerPawn::UpdateTurretRotation()
{
    if (GetTargetActor())
    {
        FVector Direction = GetTargetActor()->GetActorLocation() - TurretMesh->GetComponentLocation();
        Direction.Z = 0;
        Direction = Direction.RotateAngleAxis(-90.0f, FVector(0.0f, 0.0f, 1.0f));
        RotateTurret(Direction);
    }
}

void ATowerPawn::FireAtPlayer()
{
    if (HasAuthority())
    {
        if (GetTargetActor()) {
            MulticastFireAtPlayer();
        }
    }
}

void ATowerPawn::MulticastFireAtPlayer_Implementation()
{
        if (ProjectileClass)
        {
            FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
            FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            if (FireEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, SpawnLocation, SpawnRotation);
            }

            AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
        }
}
