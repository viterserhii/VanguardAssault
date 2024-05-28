#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TowerPawn.h"
#include "TankPawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

AMyGameMode::AMyGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsGameOver = false;
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AMyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CheckWinCondition();
    CheckLoseCondition();
}

void AMyGameMode::CheckWinCondition()
{
    if (bIsGameOver)
    {
        return;
    }

    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATowerPawn::StaticClass(), Towers);

    if (Towers.Num() == 0)
    {
        bIsGameOver = true;
        //UE_LOG(LogTemp, Warning, TEXT("You Win!"));
        ShowWinWidget();
        GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle, this, &AMyGameMode::RestartGame, 5.0f, false);
    }
}

void AMyGameMode::CheckLoseCondition()
{
    if (bIsGameOver)
    {
        return;
    }

    if (!PlayerPawn || PlayerPawn->IsPendingKill())
    {
        bIsGameOver = true;
        //UE_LOG(LogTemp, Warning, TEXT("You Lose!"));
        ShowLoseWidget();
        GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle, this, &AMyGameMode::RestartGame, 5.0f, false);
    }
}

void AMyGameMode::ShowWinWidget()
{
    if (WinWidgetClass)
    {
        WinWidget = CreateWidget<UUserWidget>(GetWorld(), WinWidgetClass);
        if (WinWidget)
        {
            WinWidget->AddToViewport();
        }
    }
}

void AMyGameMode::ShowLoseWidget()
{
    if (LoseWidgetClass)
    {
        LoseWidget = CreateWidget<UUserWidget>(GetWorld(), LoseWidgetClass);
        if (LoseWidget)
        {
            LoseWidget->AddToViewport();
        }
    }
}

void AMyGameMode::RestartGame()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
