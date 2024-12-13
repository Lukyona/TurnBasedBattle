// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UTurnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTurnComponent();

private:
	bool bIsMyTurn;
	int32 MyTurnNum;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	float OriginMovingAbility;

	float CurMovingAbility;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int32 OriginActionAbility = 1;

	int32 CurActionAbility;

public:	//Getters and setters
	bool IsMyTurn() 
	{
		return bIsMyTurn; 
	}

	int32 GetTurnNum() 
	{
		return MyTurnNum; 
	}
	void SetTurnNum(int32 TurnNum)
	{
		MyTurnNum = TurnNum;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetOriginMovingAbility() 
	{
		return OriginMovingAbility; 
	}
	void SetOriginMoveingAbility(float Value) 
	{
		OriginMovingAbility = Value; 
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurMovingAbility() 
	{
		return CurMovingAbility; 
	}

	int32 GetOriginActionAbility() 
	{
		return OriginActionAbility; 
	}
	void SetOriginActionAbility(int32 Value) 
	{
		OriginActionAbility = Value; 
	}

	int32 GetCurActionAbility() 
	{
		return CurActionAbility;
	}
	void SetCurActionAbility(int32 Value)
	{
		CurActionAbility = Value;
	}

	//Core methods
	void SetTurn(bool Turn);
	void UpdateCurMovingAbility(float MovedDist);

	bool CanMove(float Distance) const;
	bool CanAttack() const;
};
