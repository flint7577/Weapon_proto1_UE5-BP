#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DubokSurfaceType.h"
#include "DubokPenetrableWall.generated.h"

class ADubokProjectile;
class UDubokSonarTargetComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class DUBOK_API ADubokPenetrableWall : public AActor
{
    GENERATED_BODY()

public:
    ADubokPenetrableWall();

    UFUNCTION(BlueprintCallable, Category="Dubok|Wall")
    bool HandleProjectileImpact(ADubokProjectile* Projectile, const FHitResult& Hit);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Wall")
    TObjectPtr<UStaticMeshComponent> WallMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Wall")
    TObjectPtr<UDubokSonarTargetComponent> SonarTarget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Wall")
    EDubokSurfaceType SurfaceType = EDubokSurfaceType::Wood;
};
