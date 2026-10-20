#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DubokWeapon.generated.h"

class ADubokProjectile;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDubokAmmoChanged, int32, NewAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDubokPrimedChanged, bool, bNewPrimed);

UCLASS(Blueprintable)
class DUBOK_API ADubokWeapon : public AActor
{
    GENERATED_BODY()

public:
    ADubokWeapon();

    UFUNCTION(BlueprintCallable, Category="Dubok|Weapon")
    void PrimaryAttack();

    UFUNCTION(BlueprintCallable, Category="Dubok|Weapon")
    int32 RefillAmmo(int32 Amount);

    UFUNCTION(BlueprintPure, Category="Dubok|Weapon")
    int32 GetAmmo() const { return Ammo; }

    UFUNCTION(BlueprintPure, Category="Dubok|Weapon")
    bool IsPrimed() const { return bIsPrimed; }

    UPROPERTY(BlueprintAssignable, Category="Dubok|Weapon")
    FDubokAmmoChanged OnAmmoChanged;

    UPROPERTY(BlueprintAssignable, Category="Dubok|Weapon")
    FDubokPrimedChanged OnPrimedChanged;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Weapon")
    TObjectPtr<UStaticMeshComponent> WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Weapon")
    TObjectPtr<USceneComponent> Muzzle;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Weapon")
    TSubclassOf<ADubokProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Weapon", meta=(ClampMin="0"))
    int32 StartingAmmo = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Weapon", meta=(ClampMin="1"))
    int32 MaxAmmo = 99;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Weapon")
    int32 Ammo = 3;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Weapon")
    bool bIsPrimed = false;

    UFUNCTION(BlueprintImplementableEvent, Category="Dubok|Weapon")
    void PlayPrimeFeedback();

    UFUNCTION(BlueprintImplementableEvent, Category="Dubok|Weapon")
    void PlayFireFeedback();
};
