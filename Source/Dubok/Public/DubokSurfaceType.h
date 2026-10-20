#pragma once

#include "CoreMinimal.h"
#include "DubokSurfaceType.generated.h"

UENUM(BlueprintType)
enum class EDubokSurfaceType : uint8
{
    Wood,
    Kevlar,
    Concrete
};
