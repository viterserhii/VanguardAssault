#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Projectile.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "TankPawn.generated.h"

UCLASS()
class VANGUARDASSAULT_API ATankPawn : public ATurretPawn
{
    GENERATED_BODY()

public:
    ATankPawn();
    void PlayEngineSound();

    UPROPERTY(ReplicatedUsing = OnRep_Position)
    FVector ReplicatedPosition;

    UFUNCTION()
    void OnRep_Position();

    UPROPERTY(ReplicatedUsing = OnRep_Rotation)
    FRotator ReplicatedRotation;

    UFUNCTION()
    void OnRep_Rotation();

    void MoveForward(float Value);
    void UpdateMovement(float DeltaTime);
    //void UpdateEffects();
    void PredictMovement(float DeltaTime, float MoveValue, float TurnValue);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerMoveForward(float Value);

    void TurnRight(float Value);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerTurnRight(float Value);

    float CurrentAcceleration;
    float AccelerationDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TurnSpeed;

    float MoveForwardValue;
    float TurnRightValue;

    FVector PredictedPosition;
    FRotator PredictedRotation;

    ///////////////////////////////////////////////////

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Fire();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFire();

    void HandleFiring();

    UFUNCTION(NetMulticast, Unreliable)
    void MulticastFireEffects(FVector SpawnLocation, FRotator SpawnRotation);

    void ResetFire();

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
    float FireRate;

    void AddAmmo(int32 AmmoAmount);

    UPROPERTY(Replicated)
    int32 MaxAmmo;

    UPROPERTY(Replicated)
    int32 CurrentAmmo;

    ///////////////////////////////////////////////////

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void AimTowardsMousePosition();

private:
    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UFloatingPawnMovement* MovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<class AProjectile> ProjectileClass;

    bool bCanFire = true;
    FTimerHandle FireRateTimerHandle;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* FireEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* LeftDustEffect1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* LeftDustEffect2;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* RightDustEffect1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* RightDustEffect2;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* FireSoundCue;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* EngineSoundCue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* EngineAudioComponent;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* TreadSoundCue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* TreadAudioComponent;

    UFUNCTION()
    void OnHealthChanged(float HealthPercentage);

    FVector ClientPosition;
    FRotator ClientRotation;

    float LastReplicatedTime;
    float ReplicationInterval;
};
