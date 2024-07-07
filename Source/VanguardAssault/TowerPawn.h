#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "TankPawn.h"
#include "Particles/ParticleSystemComponent.h"
#include "TowerPawn.generated.h"

UCLASS()
class VANGUARDASSAULT_API ATowerPawn : public ATurretPawn
{
    GENERATED_BODY()

public:
    ATowerPawn();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    AActor* GetTargetActor() const { return TargetActor; }
    void SetTargetActor(AActor* NewTarget) { TargetActor = NewTarget; }
    USphereComponent* GetDetectionSphere() const { return DetectionSphere; }

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void FireAtPlayer();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastFireAtPlayer();
    void MulticastFireAtPlayer_Implementation();

    void UpdateTurretRotation();

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* DetectionSphere;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<AProjectile> ProjectileClass;

    FTimerHandle UpdateTimerHandle;
    FTimerHandle FireTimerHandle;
    AActor* TargetActor;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* FireEffect;
};
