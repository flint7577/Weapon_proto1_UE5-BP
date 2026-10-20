#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DubokCharacter.generated.h"

class ADubokWeapon;
class UCameraComponent;
class UChildActorComponent;
class UInputAction;
class UInputMappingContext;

UCLASS(Blueprintable)
class DUBOK_API ADubokCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ADubokCharacter();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintPure, Category="Dubok|Weapon")
    ADubokWeapon* GetWeapon() const;

    UFUNCTION(BlueprintCallable, Category="Dubok|Sonar")
    void ActivateSonar();

    UFUNCTION(BlueprintPure, Category="Dubok|Health")
    float GetHealth() const { return Health; }

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Character")
    TObjectPtr<UCameraComponent> FirstPersonCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dubok|Weapon")
    TObjectPtr<UChildActorComponent> WeaponComponent;

protected:
    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartCrouch();
    void StopCrouch();
    void StartSprint();
    void StopSprint();
    void PrimaryAttack();
    void ResetSonarCooldown();
    void Die();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputMappingContext> WeaponMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputAction> CrouchAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputAction> PrimaryAttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dubok|Input")
    TObjectPtr<UInputAction> SonarAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Movement", meta=(ClampMin="0.0"))
    float WalkSpeed = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Movement", meta=(ClampMin="0.0"))
    float SprintSpeed = 750.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Movement", meta=(ClampMin="0.0"))
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Movement", meta=(ClampMin="0.0"))
    float StaminaDrainPerSecond = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Movement", meta=(ClampMin="0.0"))
    float StaminaRecoveryPerSecond = 18.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Movement")
    float Stamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Sonar", meta=(ClampMin="1.0"))
    float SonarRadius = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Sonar", meta=(ClampMin="0.0"))
    float SonarCooldown = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dubok|Health", meta=(ClampMin="1.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dubok|Health")
    float Health = 100.0f;

    bool bSprinting = false;
    bool bSonarReady = true;
    FTimerHandle SonarCooldownTimer;
};
