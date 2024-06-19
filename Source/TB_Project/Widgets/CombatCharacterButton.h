// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatCharacterButton.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API UCombatCharacterButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	class ACharacter* Character;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> AllCharacters;

	UPROPERTY(BlueprintReadWrite)
	class UObject* Image;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCharButtonImg(const FString& Name);
	UFUNCTION(BlueprintImplementableEvent)
	void ScaleDown();
	UFUNCTION(BlueprintImplementableEvent)
	void ScaleUp();
	UFUNCTION(BlueprintImplementableEvent)
	void ButtonDown();

	UFUNCTION(BlueprintCallable)
	void SetCharacter(ACharacter* Char) { Character = Char; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACharacter* GetCharacter() { return Character; }

	UFUNCTION(BlueprintCallable)
	void AddCharacterName(class ACharacter* Char);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FString GetCharacterName(FString name);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetHealthProgressBarPercent(ACharacter* Char);
};
