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
    bGameStarted = false;
    CountdownTime = 10;
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    ShowCountdownWidget();

    DisablePlayerInput();

    GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AMyGameMode::UpdateCountdown, 1.0f, true);
}

void AMyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CheckWinCondition();
    CheckLoseCondition();
}

void AMyGameMode::ShowCountdownWidget()
{
    if (CountdownWidgetClass)
    {
        CountdownWidget = CreateWidget<UUserWidget>(GetWorld(), CountdownWidgetClass);
        if (CountdownWidget)
        {
            CountdownWidget->AddToViewport();
        }
    }
}

void AMyGameMode::UpdateCountdown()
{
    if (CountdownTime <= 0)
    {
        HideCountdownWidget();
        EnablePlayerInput();
        GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
        return;
    }

    if (CountdownWidget)
    {
        UTextBlock* CountdownText = Cast<UTextBlock>(CountdownWidget->GetWidgetFromName("CountdownText"));
        if (CountdownText)
        {
            CountdownText->SetText(FText::FromString(FString::FromInt(CountdownTime)));
        }
    }

    CountdownTime--;
}

void AMyGameMode::HideCountdownWidget()
{
    if (CountdownWidget)
    {
        CountdownWidget->RemoveFromViewport();
    }
}

void AMyGameMode::RestartGame()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AMyGameMode::EnablePlayerInput()
{
    if (PlayerPawn)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            PlayerController->SetInputMode(FInputModeGameOnly());
            PlayerController->bShowMouseCursor = false;
            bGameStarted = true;
        }
    }
}

void AMyGameMode::DisablePlayerInput()
{
    if (PlayerPawn)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            PlayerController->SetInputMode(FInputModeUIOnly());
            PlayerController->bShowMouseCursor = true;
        }
    }
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