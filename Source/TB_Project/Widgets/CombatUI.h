// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatUI.generated.h"

class UCombatCharacterButton;

UENUM(BlueprintType)
enum class EMessage : uint8
{
	NotTurn, CannotMove, WrongSpot, NoTarget, TooFarToAttack,
};

UCLASS()
class TB_PROJECT_API UCombatUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(BlueprintReadWrite)
	TArray<UCombatCharacterButton*> CombatCharacterButtons;

public: //Core methods
	void UpdateCharButtons(int32 TurnNum);

	UFUNCTION(BlueprintCallable)
	UCombatCharacterButton* GetDeadCharButton(ACharacter* Character);

	void UpdateEnemyHealth(ACharacter* Enemy);


	// BlueprintImplementableEvents
	UFUNCTION(BlueprintImplementableEvent)		// FString을 매개변수로 가지려면 무조건 이렇게 써야만 컴파일됨
	void AddCharButtons(ACharacter* Character, const FString& Name);
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveCharButton(ACharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	 void ShowEnemyInfo(const FString& Name, int32 CurHealth, int32 MaxHealth);
	UFUNCTION(BlueprintImplementableEvent)
	 void HideEnemyInfo();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void SetEnemyName(const FString& Name);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void SetEnemyHealth(int32 CurHealth, int32 MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
	 void ShowMessage(EMessage Type);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void HideMessage();

};
