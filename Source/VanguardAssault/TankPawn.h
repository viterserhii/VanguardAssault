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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 CurrentAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 MaxAmmo;

    UFUNCTION()
    void AddAmmo(int32 AmmoAmount);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void MoveForward(float Value);
    void TurnRight(float Value);
    void AimTowardsMousePosition();

private:
    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UFloatingPawnMovement* MovementComponent;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float AccelerationDuration = 2.0f;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentAcceleration)
    float CurrentAcceleration;

    UPROPERTY(Replicated)
    float DesiredAcceleration;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<class AProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float FireRate = 3.0f;

    bool bCanFire = true;

    FTimerHandle FireRateTimerHandle;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Fire();

    void ResetFire();

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

    UPROPERTY(ReplicatedUsing = OnRep_CurrentAcceleration)
    float ReplicatedCurrentAcceleration;

    UFUNCTION()
    void OnRep_CurrentAcceleration();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveForward(float Value);

    UPROPERTY(ReplicatedUsing = OnRep_CurrentTurnRate)
    float ReplicatedCurrentTurnRate;

    UFUNCTION()
    void OnRep_CurrentTurnRate();

    UPROPERTY(Replicated)
    float CurrentTurnRate;

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_TurnRight(float Value);

    //virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
