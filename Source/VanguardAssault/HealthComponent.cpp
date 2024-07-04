#include "HealthComponent.h"
#include "TurretPawn.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    MaxHealth = 100.0f;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;

    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
    }
}

void UHealthComponent::ReduceHealth(float Damage)
{
    if (Damage <= 0.0f || CurrentHealth <= 0.0f)
    {
        return;
    }

    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

    float HealthPercentage = CurrentHealth / MaxHealth;
    OnHealthChanged.Broadcast(HealthPercentage);

    if (CurrentHealth <= 0.0f)
    {
        OnRanOutOfHealth.Broadcast();

        ATurretPawn* OwnerPawn = Cast<ATurretPawn>(GetOwner());
        if (OwnerPawn)
        {
            OwnerPawn->HandleDeath();
        }
    }
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    ReduceHealth(Damage);
}