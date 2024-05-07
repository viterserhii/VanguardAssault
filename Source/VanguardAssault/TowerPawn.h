#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "TowerPawn.generated.h"


UCLASS()
class VANGUARDASSAULT_API ATowerPawn : public ATurretPawn
{
	GENERATED_BODY()

public:
    ATowerPawn();

protected:
    virtual void BeginPlay() override;

};