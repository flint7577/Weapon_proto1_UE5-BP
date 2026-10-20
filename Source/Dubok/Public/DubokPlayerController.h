#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DubokPlayerController.generated.h"

class UUserWidget;

UCLASS(Blueprintable)
class DUBOK_API ADubokPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ADubokPlayerController();

    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable, Category="Dubok|Menu")
    void TogglePauseMenu();

    UFUNCTION(BlueprintCallable, Category="Dubok|Menu")
    void ShowDeathMenu();

    UFUNCTION(BlueprintCallable, Category="Dubok|Menu")
    void ShowOptionsMenu();

    UFUNCTION(BlueprintCallable, Category="Dubok|Menu")
    void CloseMenu();

    UFUNCTION(BlueprintCallable, Category="Dubok|Menu")
    void RespawnPlayer();

    UFUNCTION(BlueprintCallable, Category="Dubok|Menu")
    void ExitGame();

protected:
    virtual void BeginPlay() override;
    void ShowMenu(TSubclassOf<UUserWidget> MenuClass, bool bPauseGame);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|UI")
    TSubclassOf<UUserWidget> CrosshairClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|UI")
    TSubclassOf<UUserWidget> DeathMenuClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|UI")
    TSubclassOf<UUserWidget> OptionsMenuClass;

    UPROPERTY()
    TObjectPtr<UUserWidget> CrosshairWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> ActiveMenu;
};
