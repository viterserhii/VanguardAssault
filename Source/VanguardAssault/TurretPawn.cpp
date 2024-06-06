#include "TurretPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameMode.h"

ATurretPawn::ATurretPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent;

    CapsuleComponent->InitCapsuleSize(22.0f, 44.0f);

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    BaseMesh->SetupAttachment(RootComponent);

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
    TurretMesh->SetupAttachment(BaseMesh);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(TurretMesh);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ATurretPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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

void ATurretPawn::RotateTurret(FVector TargetDirection)
{
    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode && !GameMode->bGameStarted)
    {
        return;
    }

    FRotator CurrentRotation = TurretMesh->GetComponentRotation();
    FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->DeltaTimeSeconds, RotationSpeed);

    TurretMesh->SetWorldRotation(NewRotation);
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

void ATurretPawn::HandleDeath()
{
    if (DeathEffect)
    {
         UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation(), GetActorRotation());
    }

    if (DeathSoundCue)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DeathSoundCue, GetActorLocation());
    }

    Destroy();
}