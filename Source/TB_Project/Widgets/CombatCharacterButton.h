// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatCharacterButton.generated.h"

/**
 * 
 */
class ACharacter;
class UObject;

UCLASS()
class TB_PROJECT_API UCombatCharacterButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* Character;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> AllCharacters;

	UPROPERTY(BlueprintReadWrite)
	UObject* Image;

public: //Getters and setters
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACharacter* GetCharacter() 
	{
		return Character; 
	}
	UFUNCTION(BlueprintCallable)
	void SetCharacter(ACharacter* InCharacter) 
	{
		Character = InCharacter;
	}

	//Core methods
	UFUNCTION(BlueprintCallable)
	void AddCharacterName(ACharacter* InCharacter);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetCharacterName(FString RawName);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealthProgressBarPercent(ACharacter* InCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCharButtonImg(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent)
	void ScaleDown();
	UFUNCTION(BlueprintImplementableEvent)
	void ScaleUp();

	UFUNCTION(BlueprintImplementableEvent)
	void ButtonDown();
};
