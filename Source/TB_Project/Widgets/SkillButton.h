// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillButton.generated.h"

/**
 * 
 */
class USkillButton;
class USkillInfoUI;
class UTexture;

UCLASS()
class TB_PROJECT_API USkillButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	USkillButton* HoveredSkill;
	USkillInfoUI* InfoUI;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSelected;
	UPROPERTY(BlueprintReadWrite)
	bool bIsHovered;

	//Skill Info
	UPROPERTY(BlueprintReadWrite)
	FString SkillName;
	UPROPERTY(BlueprintReadWrite)
	FString SkillKoreanName;
	UPROPERTY(BlueprintReadWrite)
	FString SkillExplanation;
	UPROPERTY(BlueprintReadWrite)
	int32 MinDamage;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxDamage;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAction;
	UPROPERTY(BlueprintReadWrite)
	bool bIsNeedTarget;

public: //Getters and setters
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSelectedNow() 
	{
		return bIsSelected; 
	}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsHoveredNow() 
	{
		return bIsHovered; 
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetSkillName() 
	{
		return SkillName; 
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetSkillInfo(FString& OutSkillName, FString& OutSkillExplanation, int32& OutMinDamage, int32& OutMaxDamage);

	//Core methods
	UFUNCTION(BlueprintCallable)
	void PrepareSkill();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetRandomDamage();

	void CancelSelection();
    
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSkillButtonInfo(UTexture* Img, const FString& InSkillName, const FString& InSkillKoreanName,
		const FString& InSkillExplanation, int32 InMinDamage, int32 InMaxDamage, bool InIsAction, bool InIsNeedTarget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RemoveSkillHoverEffect();
};
