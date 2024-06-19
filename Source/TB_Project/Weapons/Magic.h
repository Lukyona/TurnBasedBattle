// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PlayerWeapon.h"
#include "Magic.generated.h"

UCLASS()
class TB_PROJECT_API AMagic : public APlayerWeapon
{
	GENERATED_BODY()

protected:
	//class ACharacter* Target;

	EMagicType Type = EMagicType::None;

    UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<AActor>> MagicSkills;

public:
	AMagic();

	//void SetTarget(ACharacter* target) { Target = target; }
	
	virtual void SpawnMagic(EMagicType MagicType) override;

};
