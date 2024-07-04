#include "TowerAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

void ATowerAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTree)
    {
        UseBlackboard(BlackboardData, BlackboardComponent);
        RunBehaviorTree(BehaviorTree);
    }
}
