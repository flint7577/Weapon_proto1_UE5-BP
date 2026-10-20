#include "DubokPenetrableWall.h"

#include "DubokProjectile.h"
#include "DubokSonarTargetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ADubokPenetrableWall::ADubokPenetrableWall()
{
    PrimaryActorTick.bCanEverTick = false;

    WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
    SetRootComponent(WallMesh);
    WallMesh->SetCollisionProfileName(TEXT("BlockAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        WallMesh->SetStaticMesh(CubeMesh.Object);
    }

    SonarTarget = CreateDefaultSubobject<UDubokSonarTargetComponent>(TEXT("SonarTarget"));
}

bool ADubokPenetrableWall::HandleProjectileImpact(ADubokProjectile* Projectile, const FHitResult& Hit)
{
    if (!IsValid(Projectile))
    {
        return false;
    }

    if (SurfaceType == EDubokSurfaceType::Wood)
    {
        return true;
    }

    if (SurfaceType == EDubokSurfaceType::Kevlar && !Projectile->HasPenetratedHardMaterial())
    {
        Projectile->MarkHardMaterialPenetrated();
        return true;
    }

    return false;
}
