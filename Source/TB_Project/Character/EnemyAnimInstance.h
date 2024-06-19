// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:  
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        float Direction;

public:
	void NativeUpdateAnimation(float DeltaSeconds) override;


};
