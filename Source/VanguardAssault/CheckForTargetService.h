#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckForTargetService.generated.h"

UCLASS()
class VANGUARDASSAULT_API UCheckForTargetService : public UBTService
{
    GENERATED_BODY()

public:
    UCheckForTargetService();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    bool IsPlayerInAttackRange(UBehaviorTreeComponent& OwnerComp);

    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectionRadius = 2000.0f;
};
