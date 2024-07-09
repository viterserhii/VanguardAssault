#include "AttackTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "TowerPawn.h"

UAttackTask::UAttackTask()
{
    NodeName = "Perform Attack";
}

EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
    ATowerPawn* Tower = Cast<ATowerPawn>(ControlledPawn);

    if (Tower)
    {
        Tower->FireAtPlayer();
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
