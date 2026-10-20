#include "DubokConcreteWall.h"

#include "DubokSonarTargetComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"

ADubokConcreteWall::ADubokConcreteWall()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    IntactMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IntactMesh"));
    IntactMesh->SetupAttachment(SceneRoot);
    IntactMesh->SetCollisionProfileName(TEXT("BlockAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        IntactMesh->SetStaticMesh(CubeMesh.Object);
    }

    GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
    GeometryCollection->SetupAttachment(SceneRoot);
    GeometryCollection->SetVisibility(false);
    GeometryCollection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GeometryCollection->SetSimulatePhysics(false);

    SonarTarget = CreateDefaultSubobject<UDubokSonarTargetComponent>(TEXT("SonarTarget"));
}

void ADubokConcreteWall::BeginPlay()
{
    Super::BeginPlay();
    ShotsRequired = FMath::RandRange(FMath::Min(MinimumShots, MaximumShots), FMath::Max(MinimumShots, MaximumShots));
}

void ADubokConcreteWall::HandleProjectileImpact(const FHitResult& Hit, const FVector& ShotDirection)
{
    if (bBroken)
    {
        return;
    }

    ++ShotsTaken;
    OnConcreteDamaged(ShotsTaken, ShotsRequired);
    if (ShotsTaken < ShotsRequired)
    {
        return;
    }

    bBroken = true;
    IntactMesh->SetVisibility(false, true);
    IntactMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GeometryCollection->SetVisibility(true, true);
    GeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GeometryCollection->SetSimulatePhysics(true);
    GeometryCollection->WakeAllRigidBodies();
    GeometryCollection->ApplyExternalStrain(-1, Hit.ImpactPoint, BreakRadius, 1, 1.0f, BreakStrain);
    GeometryCollection->AddRadialImpulse(Hit.ImpactPoint, BreakRadius, BreakImpulse,
        ERadialImpulseFalloff::RIF_Linear, true);
    OnConcreteBroken();
}
