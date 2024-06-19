// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CPlayer.h"
#include "CPlayer_Sparrow.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API ACPlayer_Sparrow : public ACPlayer
{
	GENERATED_BODY()

	ACPlayer_Sparrow();

protected:
	virtual void BeginPlay() override;
	virtual void UnEquip() override;

};
