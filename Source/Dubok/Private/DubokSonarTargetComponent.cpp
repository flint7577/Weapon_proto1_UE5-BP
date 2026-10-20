#include "DubokSonarTargetComponent.h"

#include "Components/MeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UDubokSonarTargetComponent::UDubokSonarTargetComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDubokSonarTargetComponent::Reveal()
{
    if (!GetOwner() || !RevealMaterial)
    {
        return;
    }

    TArray<UMeshComponent*> Meshes;
    GetOwner()->GetComponents<UMeshComponent>(Meshes);
    for (UMeshComponent* Mesh : Meshes)
    {
        if (IsValid(Mesh))
        {
            Mesh->SetOverlayMaterial(RevealMaterial);
        }
    }

    GetWorld()->GetTimerManager().ClearTimer(RevealTimer);
    GetWorld()->GetTimerManager().SetTimer(
        RevealTimer,
        this,
        &UDubokSonarTargetComponent::ClearReveal,
        RevealDuration,
        false);
}

void UDubokSonarTargetComponent::ClearReveal()
{
    if (!GetOwner())
    {
        return;
    }

    TArray<UMeshComponent*> Meshes;
    GetOwner()->GetComponents<UMeshComponent>(Meshes);
    for (UMeshComponent* Mesh : Meshes)
    {
        if (IsValid(Mesh))
        {
            Mesh->SetOverlayMaterial(nullptr);
        }
    }
}
