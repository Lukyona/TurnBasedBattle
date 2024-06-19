// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy.h"
#include "Mushroom.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API AMushroom : public AEnemy
{
	GENERATED_BODY()

public:
	AMushroom();

	virtual void BeginPlay() override;

	
};
