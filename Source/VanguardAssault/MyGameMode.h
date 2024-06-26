#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"
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

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    bool bGameStarted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> WinWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LoseWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> CountdownWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> StartMenuWidgetClass;

    UPROPERTY()
    UUserWidget* WinWidget;

    UPROPERTY()
    UUserWidget* LoseWidget;

    UPROPERTY()
    UUserWidget* CountdownWidget;

    UPROPERTY()
    UUserWidget* StartMenuWidget;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* AmbientSoundCue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* AmbientAudioComponent;

private:
    APawn* PlayerPawn;

    FTimerHandle RestartTimerHandle;
    FTimerHandle CountdownTimerHandle;

    int32 CountdownTime;

    void ShowWinWidget();
    void ShowLoseWidget();
    void ShowCountdownWidget();
    void UpdateCountdown();
    void HideCountdownWidget();
    void RestartGame();

    UFUNCTION()
    void StartGame();

    UFUNCTION()
    void StartGameWithDelay();

    UFUNCTION()
    void ShowStartMenu();

    UFUNCTION()
    void QuitGame();

    void PlayAmbientSound();
};
