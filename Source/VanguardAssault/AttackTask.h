#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackTask.generated.h"

UCLASS()
class VANGUARDASSAULT_API UAttackTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UAttackTask();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
