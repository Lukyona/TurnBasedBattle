// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_EnemyCollision.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API UANS_EnemyCollision : public UAnimNotifyState
{
	GENERATED_BODY()
public:
    FString GetNotifyName_Implementation() const override;
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    UPROPERTY(EditAnywhere)
    bool bIsDoubleWeapon = false;
};
