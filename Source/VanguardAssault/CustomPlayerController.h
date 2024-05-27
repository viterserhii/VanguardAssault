#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class VANGUARDASSAULT_API ACustomPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Player")
    void SetPlayerEnabledState(bool bPlayerEnabled);
};
