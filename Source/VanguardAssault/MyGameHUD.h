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
    void CreateHUD();
    void ShowHUD();
    void HideHUD();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION()
    void UpdateHealthBar(float HealthPercentage);

    UFUNCTION()
    void UpdateAmmoCount(int32 CurrentAmmo, int32 MaxAmmo);

    UFUNCTION()
    void UpdateTowerCount(int32 TowerCount);

    UFUNCTION()
    void CheckTowerCount();

private:
    UPROPERTY(EditAnywhere, Category = "HUD Elements")
    TSubclassOf<class UUserWidget> PlayerHUDBPClass;

    UPROPERTY()
    class UUserWidget* PlayerHUDWidget;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AmmoText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TowerCountText;

    FTimerHandle TowerCountTimerHandle;
};
