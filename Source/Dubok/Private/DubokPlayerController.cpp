#include "DubokPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

ADubokPlayerController::ADubokPlayerController()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> Crosshair(TEXT("/Game/FirstPerson/Blueprints/WBP_Crosshair_Static"));
    static ConstructorHelpers::FClassFinder<UUserWidget> Pause(TEXT("/Game/Weapons/Proto1/UI/WBP_PauseMenu"));
    static ConstructorHelpers::FClassFinder<UUserWidget> Death(TEXT("/Game/Weapons/Proto1/UI/WBP_DeathMenu"));
    static ConstructorHelpers::FClassFinder<UUserWidget> Options(TEXT("/Game/Weapons/Proto1/UI/WBP_OptionsMenu"));
    CrosshairClass = Crosshair.Class;
    PauseMenuClass = Pause.Class;
    DeathMenuClass = Death.Class;
    OptionsMenuClass = Options.Class;
}

void ADubokPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (IsLocalController() && CrosshairClass)
    {
        CrosshairWidget = CreateWidget<UUserWidget>(this, CrosshairClass);
        if (CrosshairWidget) CrosshairWidget->AddToViewport();
    }
}

void ADubokPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ADubokPlayerController::TogglePauseMenu);
}

void ADubokPlayerController::ShowMenu(TSubclassOf<UUserWidget> MenuClass, const bool bPauseGame)
{
    CloseMenu();
    if (!MenuClass) return;
    ActiveMenu = CreateWidget<UUserWidget>(this, MenuClass);
    if (!ActiveMenu) return;

    ActiveMenu->AddToViewport(20);
    SetPause(bPauseGame);
    SetShowMouseCursor(true);
    FInputModeUIOnly Mode;
    Mode.SetWidgetToFocus(ActiveMenu->TakeWidget());
    SetInputMode(Mode);
}

void ADubokPlayerController::TogglePauseMenu()
{
    if (ActiveMenu)
    {
        CloseMenu();
    }
    else
    {
        ShowMenu(PauseMenuClass, true);
    }
}

void ADubokPlayerController::ShowDeathMenu() { ShowMenu(DeathMenuClass, true); }
void ADubokPlayerController::ShowOptionsMenu() { ShowMenu(OptionsMenuClass, true); }

void ADubokPlayerController::CloseMenu()
{
    if (ActiveMenu)
    {
        ActiveMenu->RemoveFromParent();
        ActiveMenu = nullptr;
    }
    SetPause(false);
    SetShowMouseCursor(false);
    SetInputMode(FInputModeGameOnly());
}

void ADubokPlayerController::RespawnPlayer()
{
    CloseMenu();
    if (APawn* PawnToDestroy = GetPawn())
    {
        UnPossess();
        PawnToDestroy->Destroy();
    }
    if (AGameModeBase* GameMode = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr)
    {
        GameMode->RestartPlayer(this);
    }
}

void ADubokPlayerController::ExitGame()
{
    UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}
