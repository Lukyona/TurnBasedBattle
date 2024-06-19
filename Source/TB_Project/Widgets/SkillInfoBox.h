// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillInfoBox.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API USkillInfoBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetInformation(const FString& Name, const FString& Expl, int MinDam, int MaxDam);
};
