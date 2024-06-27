#include "TurretPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameMode.h"
#include "Net/UnrealNetwork.h"

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

    TurretAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TurretAudioComponent"));
    TurretAudioComponent->SetupAttachment(RootComponent);
    TurretAudioComponent->bAutoActivate = false;

    SetReplicates(true);
    SetReplicateMovement(true);
}

void ATurretPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HasAuthority())
    {
        ReplicatedTurretRotation = TurretMesh->GetComponentRotation();
    }
    else
    {
        TurretMesh->SetWorldRotation(ReplicatedTurretRotation);
    }
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
    if (HasAuthority())
    {
        //AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        //if (GameMode && !GameMode->bGameStarted)
        //{
        //    return;
        //}

        FRotator CurrentRotation = TurretMesh->GetComponentRotation();
        FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();

        FRotator DeltaRotation = TargetRotation - CurrentRotation;
        DeltaRotation.Normalize();

        float Step = RotationSpeed * GetWorld()->DeltaTimeSeconds;

        FRotator NewRotation = CurrentRotation;

        if (FMath::Abs(DeltaRotation.Yaw) > Step)
        {
            NewRotation.Yaw += FMath::Sign(DeltaRotation.Yaw) * Step;

            if (TurretRotationSoundCue && !TurretAudioComponent->IsPlaying())
            {
                TurretAudioComponent->SetSound(TurretRotationSoundCue);
                TurretAudioComponent->Play();
            }
        }
        else
        {
            NewRotation.Yaw = TargetRotation.Yaw;

            if (TurretAudioComponent->IsPlaying())
            {
                TurretAudioComponent->Stop();
            }
        }

        TurretMesh->SetWorldRotation(NewRotation);

        TurretRotation = NewRotation;
    }
    else
    {
        Server_RotateTurret(TargetDirection);
    }
}

void ATurretPawn::OnRep_TurretRotation()
{
    TurretMesh->SetWorldRotation(ReplicatedTurretRotation);
}

void ATurretPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATurretPawn, ReplicatedTurretRotation);
}

bool ATurretPawn::Server_RotateTurret_Validate(FVector TargetDirection)
{
    return true;
}

void ATurretPawn::Server_RotateTurret_Implementation(FVector TargetDirection)
{
    RotateTurret(TargetDirection);
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
