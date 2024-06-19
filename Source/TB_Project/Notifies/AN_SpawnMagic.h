// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapons/Magic.h"
#include "AN_SpawnMagic.generated.h"


/**
 * 
 */
UCLASS()
class TB_PROJECT_API UAN_SpawnMagic : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EMagicType Type = EMagicType::None;

};
