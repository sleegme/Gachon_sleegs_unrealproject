// Copyright (c) 2024 APD Project. All Rights Reserved.
// APDCharacter.h — UE 5.4 / Enhanced Input / C++20

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "APDCharacter.generated.h" // 👈 이건 항상 마지막에!

// 전방 선언 (Forward Declarations)
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS(config = Game)
class SLEEGS_PBL_API AAPDCharacter : public ACharacter // 👈 프로젝트명에 맞춰 수정됨!
{
    GENERATED_BODY()

public:
    // 생성자: ObjectInitializer를 사용하는 최신 스타일
    explicit AAPDCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    // Getter 함수들
    FORCEINLINE USpringArmComponent* GetCameraBoom()  const noexcept { return CameraBoom; }
    FORCEINLINE UCameraComponent* GetFollowCamera() const noexcept { return FollowCamera; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // 입력 콜백
    void Input_Move(const FInputActionValue& Value);
    void Input_Look(const FInputActionValue& Value);
    void Input_JumpStarted();
    void Input_JumpCompleted();

private:
    // 카메라 컴포넌트 (TObjectPtr 사용 - UE5 권장)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    // Enhanced Input 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> IA_Move;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> IA_Look;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> IA_Jump;

    // 수치 조정 (Tuning)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (AllowPrivateAccess = "true", ClampMin = "100.0", ClampMax = "1000.0"))
    float BoomLength{ 400.f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "3.0"))
    float LookSensitivity{ 1.f };
};