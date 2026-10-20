#include "DubokProjectile.h"

#include "DubokConcreteWall.h"
#include "DubokPenetrableWall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ADubokProjectile::ADubokProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    InitialLifeSpan = 8.0f;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SetRootComponent(Collision);
    Collision->InitSphereRadius(3.0f);
    Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    Collision->SetNotifyRigidBodyCollision(true);
    Collision->OnComponentHit.AddDynamic(this, &ADubokProjectile::HandleHit);

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(Collision);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ProjectileMesh->SetRelativeScale3D(FVector(0.05f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        ProjectileMesh->SetStaticMesh(SphereMesh.Object);
    }

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->UpdatedComponent = Collision;
    Movement->InitialSpeed = 9000.0f;
    Movement->MaxSpeed = 9000.0f;
    Movement->ProjectileGravityScale = 0.05f;
    Movement->bRotationFollowsVelocity = true;
    Movement->bShouldBounce = false;
}

void ADubokProjectile::ContinueThroughActor(AActor* ActorToIgnore)
{
    if (IsValid(ActorToIgnore))
    {
        Collision->IgnoreActorWhenMoving(ActorToIgnore, true);
    }

    const FVector Direction = Movement->Velocity.GetSafeNormal();
    SetActorLocation(GetActorLocation() + Direction * ContinueOffset, false, nullptr, ETeleportType::TeleportPhysics);
}

void ADubokProjectile::HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!IsValid(OtherActor) || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator())
    {
        return;
    }

    if (ADubokPenetrableWall* Wall = Cast<ADubokPenetrableWall>(OtherActor))
    {
        if (Wall->HandleProjectileImpact(this, Hit))
        {
            ContinueThroughActor(OtherActor);
        }
        else
        {
            Destroy();
        }
        return;
    }

    if (ADubokConcreteWall* Concrete = Cast<ADubokConcreteWall>(OtherActor))
    {
        Concrete->HandleProjectileImpact(Hit, Movement->Velocity.GetSafeNormal());
        Destroy();
        return;
    }

    UGameplayStatics::ApplyPointDamage(OtherActor, Damage, Movement->Velocity.GetSafeNormal(), Hit,
        GetInstigatorController(), this, nullptr);

    if (OtherActor->ActorHasTag(TEXT("Enemy")))
    {
        ContinueThroughActor(OtherActor);
        return;
    }

    Destroy();
}
