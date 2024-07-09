#include "CheckForTargetService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "TowerPawn.h"
#include "TankPawn.h"

UCheckForTargetService::UCheckForTargetService()
{
    NodeName = "Check For Target";
}

void UCheckForTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
    ATowerPawn* Tower = Cast<ATowerPawn>(ControlledPawn);

    if (Tower && IsPlayerInAttackRange(OwnerComp))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", Tower->GetTargetActor());

        Tower->UpdateTurretRotation();
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue("Target");
    }
}

bool UCheckForTargetService::IsPlayerInAttackRange(UBehaviorTreeComponent& OwnerComp)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
    ATowerPawn* Tower = Cast<ATowerPawn>(ControlledPawn);

    if (!Tower)
    {
        return false;
    }

    TArray<AActor*> OverlappingActors;
    Tower->GetDetectionSphere()->GetOverlappingActors(OverlappingActors, ATankPawn::StaticClass());

    for (AActor* Actor : OverlappingActors)
    {
        if (ATankPawn* Player = Cast<ATankPawn>(Actor))
        {
            Tower->SetTargetActor(Player);
            return true;
        }
    }

    Tower->SetTargetActor(nullptr);
    return false;
}
