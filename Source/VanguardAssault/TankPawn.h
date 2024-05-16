#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPawn.generated.h"

UCLASS()
class VANGUARDASSAULT_API ATankPawn : public ATurretPawn
{
    GENERATED_BODY()

public:
    ATankPawn();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaTime) override;

    void MoveForward(float Value);
    void TurnRight(float Value);
    void Fire();
    void AimTowardsMousePosition();

private:
    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UFloatingPawnMovement* MovementComponent;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float AccelerationDuration = 3.0f;
    float CurrentAcceleration = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;
};
