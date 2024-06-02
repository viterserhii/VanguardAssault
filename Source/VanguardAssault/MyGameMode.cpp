#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TowerPawn.h"
#include "TankPawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

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

    ShowStartMenu();
}


void AMyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bGameStarted && !bIsGameOver)
    {
        CheckWinCondition();
        CheckLoseCondition();
    }
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
        StartGame();
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

void AMyGameMode::StartGame()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
    }

    bGameStarted = true;
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

void AMyGameMode::StartGameWithDelay()
{
    if (PlayerPawn)
    {
        PlayerPawn->SetActorHiddenInGame(false);
    }
    if (StartMenuWidget)
    {
        StartMenuWidget->RemoveFromViewport();
    }

    ShowCountdownWidget();

    GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AMyGameMode::UpdateCountdown, 1.0f, true);
}


void AMyGameMode::ShowStartMenu()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
    }

    if (PlayerPawn)
    {
        PlayerPawn->SetActorHiddenInGame(true);
    }

    if (StartMenuWidgetClass)
    {
        StartMenuWidget = CreateWidget<UUserWidget>(GetWorld(), StartMenuWidgetClass);
        if (StartMenuWidget)
        {
            StartMenuWidget->AddToViewport();

            UButton* StartButton = Cast<UButton>(StartMenuWidget->GetWidgetFromName("StartButton"));
            UButton* ExitButton = Cast<UButton>(StartMenuWidget->GetWidgetFromName("ExitButton"));
            if (StartButton)
            {
                StartButton->OnClicked.AddDynamic(this, &AMyGameMode::StartGameWithDelay);
            }
            if (ExitButton)
            {
                ExitButton->OnClicked.AddDynamic(this, &AMyGameMode::QuitGame);
            }
        }
    }
}

void AMyGameMode::QuitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void AMyGameMode::RestartGame()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
    ShowStartMenu();
}