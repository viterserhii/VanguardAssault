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

    UPROPERTY()
    UUserWidget* WinWidget;

    UPROPERTY()
    UUserWidget* LoseWidget;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* AmbientSoundCue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* AmbientAudioComponent;

    UFUNCTION(Exec)
    void ChangeTeam(int32 TeamNumber);

private:
    APawn* PlayerPawn;
    class UHealthComponent* HealthComponent;

    FTimerHandle RestartTimerHandle;

    void ShowWinWidget();
    void ShowLoseWidget();
    void RestartGame();
    void PlayAmbientSound();

    int32 NumberOfTeams; 
};
