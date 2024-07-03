#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetIsReplicated(true);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = 20000.f;
    ProjectileMovement->MaxSpeed = 20000.f;
    ProjectileMovement->SetIsReplicated(true);

    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

    Damage = 20.0f;

    bReplicates = true; 
    SetReplicateMovement(true);
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(3.0f);
}

void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        if (HitEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, Hit.Normal.Rotation());
        }

        if (HitSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(this, HitSoundCue, GetActorLocation());
        }

        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
        Destroy();
    }
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AProjectile, Damage);
}
