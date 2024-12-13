// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StateComponent.h"

UStateComponent::UStateComponent()
{
}

void UStateComponent::ChangeStateType(EStateType NewType)
{
	Type = NewType;
}

void UStateComponent::SetIdleMode()
{
	ChangeStateType(EStateType::Idle);
}

void UStateComponent::SetAttackMode()
{
	ChangeStateType(EStateType::Attack);
}

void UStateComponent::SetHittedMode()
{
	ChangeStateType(EStateType::Hitted);
}

void UStateComponent::SetDeadMode()
{
	ChangeStateType(EStateType::Dead);
}

void UStateComponent::SetWaitMode()
{
	ChangeStateType(EStateType::Wait);
}

void UStateComponent::SetMovingMode()
{
	ChangeStateType(EStateType::Moving);
}

void UStateComponent::SetPrepareMode()
{
	ChangeStateType(EStateType::Prepare);
}
