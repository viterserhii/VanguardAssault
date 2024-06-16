#include "MyGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

AMyGameHUD::AMyGameHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Blueprints/PlayerHUDBP"));
    PlayerHUDBPClass = WidgetClassFinder.Class;
}

void AMyGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (PlayerHUDBPClass)
    {
        PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDBPClass);
        if (PlayerHUDWidget)
        {
            PlayerHUDWidget->AddToViewport();

            HealthBar = Cast<UProgressBar>(PlayerHUDWidget->GetWidgetFromName(TEXT("HealthBar")));
        }
    }
}

void AMyGameHUD::UpdateHealthBar(float HealthPercentage)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(HealthPercentage);
    }
}
