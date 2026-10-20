#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DubokProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class DUBOK_API ADubokProjectile : public AActor
{
    GENERATED_BODY()

public:
    ADubokProjectile();

    UFUNCTION(BlueprintPure, Category="Dubok|Projectile")
    bool HasPenetratedHardMaterial() const { return bHardMaterialPenetrated; }

    UFUNCTION(BlueprintCallable, Category="Dubok|Projectile")
    void MarkHardMaterialPenetrated() { bHardMaterialPenetrated = true; }

    UFUNCTION(BlueprintCallable, Category="Dubok|Projectile")
    void ContinueThroughActor(AActor* ActorToIgnore);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Projectile")
    TObjectPtr<USphereComponent> Collision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Projectile")
    TObjectPtr<UStaticMeshComponent> ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Projectile")
    TObjectPtr<UProjectileMovementComponent> Movement;

protected:
    UFUNCTION()
    void HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Projectile", meta=(ClampMin="0.0"))
    float Damage = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Projectile", meta=(ClampMin="0.0"))
    float ContinueOffset = 12.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Projectile")
    bool bHardMaterialPenetrated = false;
};
