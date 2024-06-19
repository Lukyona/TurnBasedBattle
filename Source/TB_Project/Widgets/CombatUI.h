// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatUI.generated.h"

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
	TArray<class UCombatCharacterButton*> CombatCharacterButtons;


public:
	UFUNCTION(BlueprintImplementableEvent)		// FString�� �Ű������� �������� ������ �̷��� ��߸� �����ϵ�
	 void AddCharButtons(ACharacter* Character, const FString& Name);
	UFUNCTION(BlueprintImplementableEvent)
	 void RemoveCharButton(ACharacter* Character);

	void UpdateCharButtons(int Num);

	UFUNCTION(BlueprintCallable)
	class UCombatCharacterButton* GetDeadCharButton(ACharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	 void ShowEnemyInfo(const FString& Name, int CurHealth, int MaxHealth);
	UFUNCTION(BlueprintImplementableEvent)
	 void HideEnemyInfo();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void SetEnemyName(const FString& Name);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void SetEnemyHealth(int Cur, int Max);

	void UpdateEnemyHealth(ACharacter* Enemy);

	UFUNCTION(BlueprintImplementableEvent)
	 void ShowMessage(EMessage Type);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void HideMessage();

};
