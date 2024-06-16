#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyGameHUD.generated.h"

UCLASS()
class VANGUARDASSAULT_API AMyGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMyGameHUD();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION()
    void UpdateHealthBar(float HealthPercentage);

    UPROPERTY(EditAnywhere, Category = "HUD Elements")
    TSubclassOf<class UUserWidget> PlayerHUDBPClass;

    UPROPERTY()
    class UUserWidget* PlayerHUDWidget;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

private:
};
