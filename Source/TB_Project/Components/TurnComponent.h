// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UTurnComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool bMyTurn;

	int MyTurnNum;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	float OriginMovingAbility;

	float CurMovingAbility;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int OriginActionAbility = 1;

	int CurActionAbility;

public:	
	UTurnComponent();

	void SetTurn(bool Turn);
	bool IsMyTurn() { return bMyTurn; }

	void SetTurnNum(int TurnNum);
	int GetTurnNum() { return MyTurnNum; }

	void SetOriginMoveingAbility(float Value) { OriginMovingAbility = Value; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetOriginMovingAbility() { return OriginMovingAbility; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurMovingAbility() { return CurMovingAbility; }
	void UpdateCurMovingAbility(float MovedDist);

	void SetOriginActionAbility(int Value) { OriginActionAbility = Value; }
	int GetOriginActionAbility() { return OriginActionAbility; }

	void SetCurActionAbility(int value);
	int GetCurActionAbility() { return CurActionAbility; }

	bool CanMove(float Distance);
	bool CanAttack();

};
