// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CPlayer.h"
#include "CPlayer_Revenant.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API ACPlayer_Revenant : public ACPlayer
{
	GENERATED_BODY()

	ACPlayer_Revenant();
	void BeginPlay() override;
};
