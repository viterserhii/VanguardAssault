#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "TurretPawn.generated.h"

UENUM(BlueprintType)
enum class ETeamColor : uint8
{
    GreenTeam UMETA(DisplayName = "GreenTeam"),
    RedTeam   UMETA(DisplayName = "RedTeam"),
};

UCLASS()
class VANGUARDASSAULT_API ATurretPawn : public APawn
{
    GENERATED_BODY()

public:
    ATurretPawn();

protected:
    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditAnywhere, Category = "Turret")
    float RotationSpeed = 5.0f;
    void RotateTurret(FVector TargetDirection);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* ProjectileSpawnPoint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
    ETeamColor TeamColor;

private:
    void UpdateMaterialColor(UStaticMeshComponent* Mesh, int32 MaterialSlot);
    FLinearColor GetColorForTeam(ETeamColor Team) const;
};
