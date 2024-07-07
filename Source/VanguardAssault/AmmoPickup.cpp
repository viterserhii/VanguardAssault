#include "AmmoPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "TankPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

AAmmoPickup::AAmmoPickup()
{
    PrimaryActorTick.bCanEverTick = true;

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

    bReplicates = true;
    bIsAvailable = true;
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
            MulticastPlayPickupSound();
        }

        bIsAvailable = false;
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);

        if (HasAuthority())
        {
            GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AAmmoPickup::Respawn, RespawnTime, false);
        }
    }
}

void AAmmoPickup::Respawn()
{
    bIsAvailable = true;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}

void AAmmoPickup::OnRep_IsAvailable()
{
    SetActorHiddenInGame(!bIsAvailable);
    SetActorEnableCollision(bIsAvailable);
}

void AAmmoPickup::MulticastPlayPickupSound_Implementation()
{
    UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}

void AAmmoPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAmmoPickup, bIsAvailable);
}
