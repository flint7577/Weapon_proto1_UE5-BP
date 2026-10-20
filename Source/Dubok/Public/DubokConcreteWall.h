#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DubokConcreteWall.generated.h"

class UDubokSonarTargetComponent;
class UGeometryCollectionComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class DUBOK_API ADubokConcreteWall : public AActor
{
    GENERATED_BODY()

public:
    ADubokConcreteWall();

    UFUNCTION(BlueprintCallable, Category="Dubok|Concrete")
    void HandleProjectileImpact(const FHitResult& Hit, const FVector& ShotDirection);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Concrete")
    TObjectPtr<UStaticMeshComponent> IntactMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Concrete")
    TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Concrete")
    TObjectPtr<UDubokSonarTargetComponent> SonarTarget;

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent, Category="Dubok|Concrete")
    void OnConcreteDamaged(int32 CurrentShots, int32 RequiredShots);

    UFUNCTION(BlueprintImplementableEvent, Category="Dubok|Concrete")
    void OnConcreteBroken();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Concrete", meta=(ClampMin="1"))
    int32 MinimumShots = 4;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Concrete", meta=(ClampMin="1"))
    int32 MaximumShots = 8;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Concrete", meta=(ClampMin="1.0"))
    float BreakRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Concrete", meta=(ClampMin="0.0"))
    float BreakStrain = 500000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Concrete", meta=(ClampMin="0.0"))
    float BreakImpulse = 900.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Concrete")
    int32 ShotsTaken = 0;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Concrete")
    int32 ShotsRequired = 4;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Concrete")
    bool bBroken = false;
};
