#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TowerPawn.h"
#include "TankPawn.h"
#include "MyGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HealthComponent.h"

AMyGameMode::AMyGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsGameOver = false;
    bGameStarted = false;

    AmbientAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbientAudioComponent"));
    AmbientAudioComponent->bAutoActivate = false;

    NumberOfTeams = 4;
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        bGameStarted = true;
        PlayAmbientSound();
    }
}

void AMyGameMode::PlayAmbientSound()
{
    if (HasAuthority())
    {
        AmbientAudioComponent->SetSound(AmbientSoundCue);
        AmbientAudioComponent->Play();
    }
}

void AMyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bGameStarted && !bIsGameOver)
    {
        if (HasAuthority())
        {
            CheckWinCondition();
            CheckLoseCondition();
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
        ShowWinWidget();
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

void AMyGameMode::CheckLoseCondition()
{
    //if (bIsGameOver)
    //{
    //    return;
    //}

    //if (!PlayerPawn || (HealthComponent && HealthComponent->GetCurrentHealth() <= 0.0f))
    //{
    //    bIsGameOver = true;
    //    ShowLoseWidget();
    //    GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle, this, &AMyGameMode::RestartGame, 5.0f, false);
    //}
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
    if (HasAuthority())
    {
        UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
    }
}

void AMyGameMode::ChangeTeam(int32 TeamNumber)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        ATankPawn* PlayerTank = Cast<ATankPawn>(PlayerController->GetPawn());
        if (PlayerTank)
        {
            int32 ClampedTeam = FMath::Clamp(TeamNumber, 1, NumberOfTeams);
            PlayerTank->SetTeam(ClampedTeam);
        }
    }
}