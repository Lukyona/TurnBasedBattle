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
	int Damage;

public:
	void SetDamage(int value) { Damage = value; }
	
};
