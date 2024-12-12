// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/PlayerWeaponComponent.h"
#include "Components/FeetComponent.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    void NativeBeginPlay() override;
    void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    float Direction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    EWeaponType WeaponType = EWeaponType::UnArmed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FFeetData FeetData;

private:
    class ACharacter* OwnerCharacter;

    UFUNCTION()
    void  OnWeaponTypeChanged(EWeaponType NewType);
};
