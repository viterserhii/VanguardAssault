#include "AmmoPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "TankPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AAmmoPickup::AAmmoPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
    RotatingMovementComponent->RotationRate = FRotator(0, 60, 0);

    AmmoAmount = 10;
    RespawnTime = 30.0f;

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlapBegin);
}

void AAmmoPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AAmmoPickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATankPawn* Tank = Cast<ATankPawn>(OtherActor);
    if (Tank && Tank->CurrentAmmo < Tank->MaxAmmo)
    {
        int32 AmmoToAdd = FMath::Min(AmmoAmount, Tank->MaxAmmo - Tank->CurrentAmmo);
        Tank->AddAmmo(AmmoToAdd);

        if (PickupSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
        }

        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);

        GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AAmmoPickup::Respawn, RespawnTime, false);
    }
}

void AAmmoPickup::Respawn()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}
