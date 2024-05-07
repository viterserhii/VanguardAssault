#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "TankPawn.generated.h"


UCLASS()
class VANGUARDASSAULT_API ATankPawn : public ATurretPawn
{
	GENERATED_BODY()
	
public:
    ATankPawn();

protected:
    virtual void BeginPlay() override;

};