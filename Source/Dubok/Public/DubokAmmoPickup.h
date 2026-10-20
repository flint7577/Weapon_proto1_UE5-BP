#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DubokAmmoPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class DUBOK_API ADubokAmmoPickup : public AActor
{
    GENERATED_BODY()

public:
    ADubokAmmoPickup();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Pickup")
    TObjectPtr<USphereComponent> Trigger;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Pickup")
    TObjectPtr<UStaticMeshComponent> PickupMesh;

protected:
    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Pickup", meta=(ClampMin="1"))
    int32 AmmoAmount = 3;
};
