#include "DubokAmmoPickup.h"

#include "DubokCharacter.h"
#include "DubokWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ADubokAmmoPickup::ADubokAmmoPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);
    Trigger->InitSphereRadius(65.0f);
    Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADubokAmmoPickup::HandleOverlap);

    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    PickupMesh->SetupAttachment(Trigger);
    PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PickupMesh->SetRelativeScale3D(FVector(0.35f, 0.25f, 0.2f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        PickupMesh->SetStaticMesh(CubeMesh.Object);
    }
}

void ADubokAmmoPickup::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    ADubokCharacter* Character = Cast<ADubokCharacter>(OtherActor);
    ADubokWeapon* Weapon = Character ? Character->GetWeapon() : nullptr;
    if (Weapon && Weapon->RefillAmmo(AmmoAmount) > 0)
    {
        Destroy();
    }
}
