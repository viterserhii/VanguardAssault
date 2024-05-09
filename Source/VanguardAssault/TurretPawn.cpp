#include "TurretPawn.h"

ATurretPawn::ATurretPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    BaseMesh->SetupAttachment(RootComponent);

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
    TurretMesh->SetupAttachment(BaseMesh);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ATurretPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ATurretPawn::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    UpdateMaterialColor(BaseMesh, 1);
    UpdateMaterialColor(TurretMesh, 1);
}

void ATurretPawn::UpdateMaterialColor(UStaticMeshComponent* Mesh, int32 MaterialSlot)
{
    UMaterialInstanceDynamic* DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(MaterialSlot);
    if (DynamicMaterial)
    {
        FLinearColor Color = GetColorForTeam(TeamColor);
        DynamicMaterial->SetVectorParameterValue(TEXT("TeamColor"), Color);
    }
}

FLinearColor ATurretPawn::GetColorForTeam(ETeamColor Team) const
{
    switch (Team)
    {
    case ETeamColor::GreenTeam:
        return FLinearColor::Green;
    case ETeamColor::RedTeam:
        return FLinearColor::Red;
    default:
        return FLinearColor::White;
    }
}
