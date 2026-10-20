#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DubokEnemy.generated.h"

class UDubokSonarTargetComponent;

UCLASS(Blueprintable)
class DUBOK_API ADubokEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    ADubokEnemy();

    virtual void Tick(float DeltaSeconds) override;
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Enemy")
    TObjectPtr<UDubokSonarTargetComponent> SonarTarget;

protected:
    virtual void BeginPlay() override;
    void ResetAttack();
    void Die();

    UFUNCTION(BlueprintImplementableEvent, Category="Dubok|Enemy")
    void OnEnemyDamaged(float CurrentHealth);

    UFUNCTION(BlueprintImplementableEvent, Category="Dubok|Enemy")
    void OnEnemyDied();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Enemy", meta=(ClampMin="1.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Enemy", meta=(ClampMin="0.0"))
    float ChaseRange = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Enemy", meta=(ClampMin="0.0"))
    float AttackRange = 135.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Enemy", meta=(ClampMin="0.0"))
    float AttackDamage = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Enemy", meta=(ClampMin="0.01"))
    float AttackCooldown = 1.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Enemy")
    float Health = 100.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Enemy")
    bool bDead = false;

    bool bCanAttack = true;
    FTimerHandle AttackTimer;
};
