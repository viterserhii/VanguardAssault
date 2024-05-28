#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MyGameMode.generated.h"

UCLASS()
class VANGUARDASSAULT_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyGameMode();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    bool bIsGameOver;

    UFUNCTION(BlueprintCallable, Category = "Game")
    void CheckWinCondition();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void CheckLoseCondition();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> WinWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LoseWidgetClass;

    UPROPERTY()
    UUserWidget* WinWidget;

    UPROPERTY()
    UUserWidget* LoseWidget;

private:
    APawn* PlayerPawn;

    FTimerHandle RestartTimerHandle;

    void ShowWinWidget();
    void ShowLoseWidget();
    void RestartGame();
};
