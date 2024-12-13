// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PlayerWeapon.h"
#include "Bow.generated.h"

/**
 * 
 */
class AArrow;

UCLASS()
class TB_PROJECT_API ABow : public APlayerWeapon
{
	GENERATED_BODY()

public:
	ABow();
	virtual void SpawnProjectile() override;
	virtual void Shoot() override;
	virtual void UnEquip() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AArrow> ArrowClass;

	AArrow* Arrow;
	
};
