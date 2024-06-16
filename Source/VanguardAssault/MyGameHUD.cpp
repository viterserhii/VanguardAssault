#include "MyGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "TowerPawn.h"
#include "MyGameMode.h"

AMyGameHUD::AMyGameHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Blueprints/PlayerHUDBP"));
    PlayerHUDBPClass = WidgetClassFinder.Class;
}

void AMyGameHUD::BeginPlay()
{
    Super::BeginPlay();
    CreateHUD();
    HideHUD();
}

void AMyGameHUD::CreateHUD()
{
    if (PlayerHUDBPClass)
    {
        PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDBPClass);
        if (PlayerHUDWidget)
        {
            PlayerHUDWidget->AddToViewport();

            HealthBar = Cast<UProgressBar>(PlayerHUDWidget->GetWidgetFromName(TEXT("HealthBar")));
            AmmoText = Cast<UTextBlock>(PlayerHUDWidget->GetWidgetFromName(TEXT("AmmoText")));
            TowerCountText = Cast<UTextBlock>(PlayerHUDWidget->GetWidgetFromName(TEXT("TowerCountText")));

            GetWorld()->GetTimerManager().SetTimer(TowerCountTimerHandle, this, &AMyGameHUD::CheckTowerCount, 1.0f, true);
        }
    }
}

void AMyGameHUD::ShowHUD()
{
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMyGameHUD::HideHUD()
{
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMyGameHUD::UpdateHealthBar(float HealthPercentage)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(HealthPercentage);
    }
}

void AMyGameHUD::UpdateAmmoCount(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
    }
}

void AMyGameHUD::UpdateTowerCount(int32 TowerCount)
{
    if (TowerCountText)
    {
        TowerCountText->SetText(FText::FromString(FString::Printf(TEXT("Towers: %d"), TowerCount)));
    }
}

void AMyGameHUD::CheckTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATowerPawn::StaticClass(), Towers);

    UpdateTowerCount(Towers.Num());
}
