#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Components/SphereComponent.h"
#include "TankPawn.h"
#include "TowerPawn.generated.h"


UCLASS()
class VANGUARDASSAULT_API ATowerPawn : public ATurretPawn
{
    GENERATED_BODY()

public:
    ATowerPawn();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* DetectionSphere;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float FireInterval = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float UpdateInterval = 0.01f;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void FireAtPlayer();

    void UpdateTurretRotation();

    FTimerHandle UpdateTimerHandle;
    FTimerHandle FireTimerHandle;
    AActor* TargetActor;
};
