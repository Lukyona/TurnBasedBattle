// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

UENUM()
enum class EStateType
{
	Idle, Attack, Hitted, Dead, Wait, Moving, Prepare,
};

UENUM()
enum class EBuffType
{
	None, Shield, buff1, buff2, buff3, buff4,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStateComponent();

private:
    UPROPERTY(VisibleAnywhere)
	EStateType Type = EStateType::Idle;

	UPROPERTY(VisibleAnywhere)
	EBuffType CurBuffType = EBuffType::None;

public:	//Getters and setters
	bool IsIdleMode() 
	{
		return Type == EStateType::Idle; 
	}
	bool IsAttackMode()
	{
		return Type == EStateType::Attack; 
	}
	bool IsHittedMode() 
	{
		return Type == EStateType::Hitted; 
	}
	bool IsDeadMode() 
	{
		return Type == EStateType::Dead; 
	}
	bool IsWaitMode() 
	{
		return Type == EStateType::Wait; 
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMovingMode() 
	{
		return Type == EStateType::Moving;
	}
	bool IsPrepareMode() 
	{
		return Type == EStateType::Prepare; 
	}
	bool IsShieldOn() 
	{ 
		return CurBuffType == EBuffType::Shield; 
	}

	void SetBuffMode(EBuffType BuffType) 
	{
		CurBuffType = BuffType; 
	}

	void ChangeStateType(EStateType NewType);
		
	void SetIdleMode();
	void SetAttackMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetWaitMode();
	void SetMovingMode();
	void SetPrepareMode();

};
