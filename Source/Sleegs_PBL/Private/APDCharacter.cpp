// Copyright (c) 2024 APD Project. All Rights Reserved.
#include "APDCharacter.h"

// 式式 UE Includes 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY_STATIC(LogAPDCharacter, Log, All);

AAPDCharacter::AAPDCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    auto* MovComp = GetCharacterMovement();
    MovComp->bOrientRotationToMovement = true;
    MovComp->RotationRate = FRotator(0.f, 500.f, 0.f);
    MovComp->JumpZVelocity = 700.f;
    MovComp->AirControl = 0.35f;
    MovComp->MaxWalkSpeed = 500.f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = BoomLength;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SocketOffset = FVector(0.f, 0.f, 60.f);

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    PrimaryActorTick.bCanEverTick = false;
}

void AAPDCharacter::BeginPlay()
{
    Super::BeginPlay();

    const auto* PC = Cast<APlayerController>(Controller);
    if (PC && DefaultMappingContext)
    {
        if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AAPDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    auto* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    if (IA_Move) EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AAPDCharacter::Input_Move);
    if (IA_Look) EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AAPDCharacter::Input_Look);
    if (IA_Jump)
    {
        EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AAPDCharacter::Input_JumpStarted);
        EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AAPDCharacter::Input_JumpCompleted);
    }
}

void AAPDCharacter::Input_Move(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D Axis = Value.Get<FVector2D>();
    const FRotator ControlRotation{ 0.f, Controller->GetControlRotation().Yaw, 0.f };
    const FRotationMatrix RotMatrix{ ControlRotation };

    if (!FMath::IsNearlyZero(Axis.Y)) AddMovementInput(RotMatrix.GetUnitAxis(EAxis::X), Axis.Y);
    if (!FMath::IsNearlyZero(Axis.X)) AddMovementInput(RotMatrix.GetUnitAxis(EAxis::Y), Axis.X);
}

void AAPDCharacter::Input_Look(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D Axis = Value.Get<FVector2D>() * LookSensitivity;
    AddControllerYawInput(Axis.X);
    AddControllerPitchInput(Axis.Y);
}

void AAPDCharacter::Input_JumpStarted() { Jump(); }
void AAPDCharacter::Input_JumpCompleted() { StopJumping(); }