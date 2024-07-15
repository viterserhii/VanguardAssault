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
        TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), ReplicatedTurretRotation, DeltaTime, 5.0f));
    }
}

void ATurretPawn::BeginPlay()
{
    Super::BeginPlay();
    UpdateMaterialColors();
}

void ATurretPawn::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    UpdateMaterialColors();
}

void ATurretPawn::UpdateMaterialColors()
{
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

void ATurretPawn::SetTeam(int32 NewTeam)
{
    if (TeamColor != NewTeam)
    {
        TeamColor = NewTeam;
        UpdateMaterialColors();

        UE_LOG(LogTemp, Warning, TEXT("Team changed to: %d"), NewTeam);
    }
}

FLinearColor ATurretPawn::GetColorForTeam(int32 Team) const
{
    switch (Team)
    {
    case 1:
        return FLinearColor::Green;
    case 2:
        return FLinearColor::Red;
    case 3:
        return FLinearColor::Blue;
    case 4:
        return FLinearColor::Yellow;
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
