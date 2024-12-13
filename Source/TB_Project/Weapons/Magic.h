// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PlayerWeapon.h"
#include "Magic.generated.h"

UCLASS()
class TB_PROJECT_API AMagic : public APlayerWeapon
{
	GENERATED_BODY()

public:
	AMagic();

	virtual void SpawnMagic(EMagicType MagicType) override;

protected:
	EMagicType Type = EMagicType::None;

    UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<AActor>> MagicSkills;

};
