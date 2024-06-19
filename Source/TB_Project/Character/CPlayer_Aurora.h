// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CPlayer.h"
#include "CPlayer_Aurora.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API ACPlayer_Aurora : public ACPlayer
{
	GENERATED_BODY()
	
	ACPlayer_Aurora();

protected:
	virtual void BeginPlay() override;
};
