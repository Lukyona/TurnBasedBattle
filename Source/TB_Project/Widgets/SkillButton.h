// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillButton.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API USkillButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bSelected;
	UPROPERTY(BlueprintReadWrite)
	bool bHovered;
	UPROPERTY(BlueprintReadWrite)
	FString SkillName;
	UPROPERTY(BlueprintReadWrite)
	FString SkillKoreanName;
	UPROPERTY(BlueprintReadWrite)
	FString SkillExplanation;
	UPROPERTY(BlueprintReadWrite)
	int MinDamage;
	UPROPERTY(BlueprintReadWrite)
	int MaxDamage;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAction;
	UPROPERTY(BlueprintReadWrite)
	bool bNeedTarget;

	class USkillButton* HoveredSkill;
	class USkillInfoUI* InfoUI;


public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSkillButtonInfo(class UTexture* Img, const FString& _SkillName, const FString& _SkillKoreanName, const FString& _SkillExplanation,
		int _MinDamage, int _MaxDamage, bool _IsAction, bool _NeedTarget);
	
	UFUNCTION(BlueprintCallable)
	void PrepareSkill();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSelectedNow() { return bSelected; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsHoveredNow() { return bHovered; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetSkillName() { return SkillName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		void GetSkillInfo(FString& Skill_Name, FString& Skill_Explanation, int& Min_Dam, int& Max_Dam);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetRandomDamage();

	void CancelSelection();
    
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void RemoveSkillHoverEffect();
};
