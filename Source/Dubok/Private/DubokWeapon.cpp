#include "DubokWeapon.h"

#include "DubokProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

ADubokWeapon::ADubokWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
    Muzzle->SetupAttachment(WeaponMesh);
    Muzzle->SetRelativeLocation(FVector(40.0, 0.0, 0.0));

    ProjectileClass = ADubokProjectile::StaticClass();
}

void ADubokWeapon::BeginPlay()
{
    Super::BeginPlay();
    Ammo = FMath::Clamp(StartingAmmo, 0, MaxAmmo);
    OnAmmoChanged.Broadcast(Ammo);
    OnPrimedChanged.Broadcast(bIsPrimed);
}

void ADubokWeapon::PrimaryAttack()
{
    if (Ammo <= 0)
    {
        bIsPrimed = false;
        OnPrimedChanged.Broadcast(false);
        return;
    }

    if (!bIsPrimed)
    {
        bIsPrimed = true;
        OnPrimedChanged.Broadcast(true);
        PlayPrimeFeedback();
        return;
    }

    if (ProjectileClass && GetWorld())
    {
        FActorSpawnParameters Parameters;
        Parameters.Owner = this;
        Parameters.Instigator = GetInstigator();
        Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        GetWorld()->SpawnActor<ADubokProjectile>(ProjectileClass, Muzzle->GetComponentTransform(), Parameters);
    }

    Ammo = FMath::Max(0, Ammo - 1);
    bIsPrimed = false;
    OnAmmoChanged.Broadcast(Ammo);
    OnPrimedChanged.Broadcast(false);
    PlayFireFeedback();
}

int32 ADubokWeapon::RefillAmmo(const int32 Amount)
{
    if (Amount <= 0)
    {
        return 0;
    }

    const int32 PreviousAmmo = Ammo;
    Ammo = FMath::Clamp(Ammo + Amount, 0, MaxAmmo);
    const int32 Added = Ammo - PreviousAmmo;
    if (Added > 0)
    {
        OnAmmoChanged.Broadcast(Ammo);
    }
    return Added;
}
