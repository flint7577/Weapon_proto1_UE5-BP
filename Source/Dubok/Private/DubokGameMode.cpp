#include "DubokGameMode.h"

#include "DubokCharacter.h"
#include "DubokPlayerController.h"

ADubokGameMode::ADubokGameMode()
{
    DefaultPawnClass = ADubokCharacter::StaticClass();
    PlayerControllerClass = ADubokPlayerController::StaticClass();
}
