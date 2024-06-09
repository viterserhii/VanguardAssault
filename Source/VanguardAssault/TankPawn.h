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
    float CurrentAcceleration = 0.0f;

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
};
