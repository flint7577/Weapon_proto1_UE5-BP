#include "DubokCharacter.h"

#include "DubokPlayerController.h"
#include "DubokSonarTargetComponent.h"
#include "DubokWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ADubokCharacter::ADubokCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(-10.0f, 0.0f, 64.0f));
    FirstPersonCamera->bUsePawnControlRotation = true;

    WeaponComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
    WeaponComponent->SetupAttachment(FirstPersonCamera);
    WeaponComponent->SetRelativeLocation(FVector(28.0f, 18.0f, -18.0f));
    WeaponComponent->SetChildActorClass(ADubokWeapon::StaticClass());

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultContext(TEXT("/Game/Input/IMC_Default.IMC_Default"));
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> ProtoContext(TEXT("/Game/Weapons/Proto1/Input/IMC_Proto1.IMC_Proto1"));
    static ConstructorHelpers::FObjectFinder<UInputAction> MoveAsset(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
    static ConstructorHelpers::FObjectFinder<UInputAction> LookAsset(TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
    static ConstructorHelpers::FObjectFinder<UInputAction> JumpAsset(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
    static ConstructorHelpers::FObjectFinder<UInputAction> CrouchAsset(TEXT("/Game/Input/Actions/IA_Crouch.IA_Crouch"));
    static ConstructorHelpers::FObjectFinder<UInputAction> SprintAsset(TEXT("/Game/Input/Actions/IA_Sprint.IA_Sprint"));
    static ConstructorHelpers::FObjectFinder<UInputAction> PrimaryAsset(TEXT("/Game/Weapons/Proto1/Input/IA_Proto1Primary.IA_Proto1Primary"));
    static ConstructorHelpers::FObjectFinder<UInputAction> SonarAsset(TEXT("/Game/Weapons/Proto1/Input/IA_Proto1Secondary.IA_Proto1Secondary"));

    DefaultMappingContext = DefaultContext.Object;
    WeaponMappingContext = ProtoContext.Object;
    MoveAction = MoveAsset.Object;
    LookAction = LookAsset.Object;
    JumpAction = JumpAsset.Object;
    CrouchAction = CrouchAsset.Object;
    SprintAction = SprintAsset.Object;
    PrimaryAttackAction = PrimaryAsset.Object;
    SonarAction = SonarAsset.Object;
}

void ADubokCharacter::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
    Stamina = MaxStamina;

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
            if (WeaponMappingContext)
            {
                Subsystem->AddMappingContext(WeaponMappingContext, 1);
            }
        }
    }
}

void ADubokCharacter::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bSprinting && GetVelocity().SizeSquared2D() > 1.0f && Stamina > 0.0f)
    {
        Stamina = FMath::Max(0.0f, Stamina - StaminaDrainPerSecond * DeltaSeconds);
        if (Stamina <= 0.0f)
        {
            StopSprint();
        }
    }
    else if (!bSprinting)
    {
        Stamina = FMath::Min(MaxStamina, Stamina + StaminaRecoveryPerSecond * DeltaSeconds);
    }
}

void ADubokCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!Input)
    {
        return;
    }

    if (MoveAction) Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADubokCharacter::Move);
    if (LookAction) Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADubokCharacter::Look);
    if (JumpAction)
    {
        Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    }
    if (CrouchAction)
    {
        Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADubokCharacter::StartCrouch);
        Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ADubokCharacter::StopCrouch);
    }
    if (SprintAction)
    {
        Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ADubokCharacter::StartSprint);
        Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADubokCharacter::StopSprint);
    }
    if (PrimaryAttackAction) Input->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &ADubokCharacter::PrimaryAttack);
    if (SonarAction) Input->BindAction(SonarAction, ETriggerEvent::Started, this, &ADubokCharacter::ActivateSonar);
}

void ADubokCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>();
    AddMovementInput(GetActorForwardVector(), Axis.Y);
    AddMovementInput(GetActorRightVector(), Axis.X);
}

void ADubokCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>();
    AddControllerYawInput(Axis.X);
    AddControllerPitchInput(Axis.Y);
}

void ADubokCharacter::StartCrouch() { Crouch(); }
void ADubokCharacter::StopCrouch() { UnCrouch(); }

void ADubokCharacter::StartSprint()
{
    if (Stamina <= 0.0f) return;
    bSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ADubokCharacter::StopSprint()
{
    bSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

ADubokWeapon* ADubokCharacter::GetWeapon() const
{
    return WeaponComponent ? Cast<ADubokWeapon>(WeaponComponent->GetChildActor()) : nullptr;
}

void ADubokCharacter::PrimaryAttack()
{
    if (ADubokWeapon* Weapon = GetWeapon())
    {
        Weapon->PrimaryAttack();
    }
}

void ADubokCharacter::ActivateSonar()
{
    if (!bSonarReady || !GetWorld()) return;
    bSonarReady = false;

    TArray<FOverlapResult> Overlaps;
    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjectQuery.AddObjectTypesToQuery(ECC_Pawn);
    GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, ObjectQuery,
        FCollisionShape::MakeSphere(SonarRadius));

    TSet<AActor*> RevealedActors;
    for (const FOverlapResult& Overlap : Overlaps)
    {
        AActor* Actor = Overlap.GetActor();
        if (!IsValid(Actor) || RevealedActors.Contains(Actor)) continue;
        RevealedActors.Add(Actor);
        if (UDubokSonarTargetComponent* Target = Actor->FindComponentByClass<UDubokSonarTargetComponent>())
        {
            Target->Reveal();
        }
    }

    GetWorldTimerManager().SetTimer(SonarCooldownTimer, this, &ADubokCharacter::ResetSonarCooldown,
        SonarCooldown, false);
}

void ADubokCharacter::ResetSonarCooldown() { bSonarReady = true; }

float ADubokCharacter::TakeDamage(const float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Health = FMath::Max(0.0f, Health - Applied);
    if (Health <= 0.0f) Die();
    return Applied;
}

void ADubokCharacter::Die()
{
    DisableInput(Cast<APlayerController>(Controller));
    if (ADubokPlayerController* PlayerController = Cast<ADubokPlayerController>(Controller))
    {
        PlayerController->ShowDeathMenu();
    }
}
