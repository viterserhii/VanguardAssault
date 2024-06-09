#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
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
    virtual void Tick(float DeltaTime) override;
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
    float RotationSpeed = 30.0f;
    void RotateTurret(FVector TargetDirection);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* ProjectileSpawnPoint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
    ETeamColor TeamColor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComponent;

    void HandleDeath();

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* DeathEffect;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* DeathSoundCue;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* TurretRotationSoundCue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* TurretAudioComponent;

private:
    void UpdateMaterialColor(UStaticMeshComponent* Mesh, int32 MaterialSlot);
    FLinearColor GetColorForTeam(ETeamColor Team) const;
};
