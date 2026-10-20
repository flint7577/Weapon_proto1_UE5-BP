#include "DubokEnemy.h"

#include "DubokCharacter.h"
#include "DubokSonarTargetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADubokEnemy::ADubokEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    Tags.Add(TEXT("Enemy"));
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    SonarTarget = CreateDefaultSubobject<UDubokSonarTargetComponent>(TEXT("SonarTarget"));
}

void ADubokEnemy::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
}

void ADubokEnemy::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bDead) return;

    ADubokCharacter* Player = Cast<ADubokCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Player) return;

    const FVector Offset = Player->GetActorLocation() - GetActorLocation();
    const float Distance = Offset.Size2D();
    if (Distance <= ChaseRange && Distance > AttackRange)
    {
        AddMovementInput(Offset.GetSafeNormal2D());
    }
    else if (Distance <= AttackRange && bCanAttack)
    {
        bCanAttack = false;
        UGameplayStatics::ApplyDamage(Player, AttackDamage, GetController(), this, nullptr);
        GetWorldTimerManager().SetTimer(AttackTimer, this, &ADubokEnemy::ResetAttack, AttackCooldown, false);
    }
}

void ADubokEnemy::ResetAttack() { bCanAttack = true; }

float ADubokEnemy::TakeDamage(const float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    if (bDead) return 0.0f;
    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Health = FMath::Max(0.0f, Health - Applied);
    OnEnemyDamaged(Health);
    if (Health <= 0.0f) Die();
    return Applied;
}

void ADubokEnemy::Die()
{
    bDead = true;
    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    OnEnemyDied();
}
