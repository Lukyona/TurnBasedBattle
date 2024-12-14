// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageAmountText.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API UDamageAmountText : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	int32 Damage = 0;

public:
	int32 GetDamage()
	{
		return Damage;
	}
	void SetDamage(int32 value) 
	{
		Damage = value; 
	}
	
};
