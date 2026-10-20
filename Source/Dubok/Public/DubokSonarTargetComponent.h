#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DubokSonarTargetComponent.generated.h"

class UMaterialInterface;

UCLASS(ClassGroup=(Dubok), meta=(BlueprintSpawnableComponent))
class DUBOK_API UDubokSonarTargetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDubokSonarTargetComponent();

    UFUNCTION(BlueprintCallable, Category="Dubok|Sonar")
    void Reveal();

    UFUNCTION(BlueprintCallable, Category="Dubok|Sonar")
    void ClearReveal();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dubok|Sonar")
    TObjectPtr<UMaterialInterface> RevealMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dubok|Sonar", meta=(ClampMin="0.01"))
    float RevealDuration = 0.7f;

private:
    FTimerHandle RevealTimer;
};
