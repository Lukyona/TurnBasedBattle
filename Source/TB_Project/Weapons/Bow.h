// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PlayerWeapon.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API ABow : public APlayerWeapon
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AArrow> ArrowClass;
	AArrow* Arrow;

public:
	ABow();

	virtual void SpawnProjectile() override;
	virtual void Shoot() override;

	virtual void UnEquip() override;

	
};
