// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TurnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Character/Enemy.h"
#include "Components/PlayerWeaponComponent.h"
#include "Widgets/BasicUI.h"

UTurnComponent::UTurnComponent()
{
	
}

void UTurnComponent::SetTurn(bool Turn)
{
	bMyTurn = Turn;

	SetCurActionAbility(0);
	CurMovingAbility = OriginMovingAbility;

	if (!bMyTurn) return;

	if (GetOwner()->IsA<ACPlayer>())
	{
		MAINPC->ChangePlayer(Cast<ACPlayer>(GetOwner()));

		// 무기 장착 상태 확인
		UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(GetOwner());
		if (weaponComp && !weaponComp->GetEquippedWeapon())
		{
			weaponComp->SetMode(weaponComp->GetCurrentWeaponType());
		}

		MAINPC->GetBasicUI()->ResetMovingAbilityBarPercent();
		MAINPC->GetBasicUI()->EnableSkillButtons();
		MAINPC->GetBasicUI()->EnableEndTurnButton();

	}
	else if (GetOwner()->IsA<AEnemy>())
	{
		// UAIBlueprintHelperLibrary 사용하려면 Build.cs에 AIModule 추가할 것.
		UAIBlueprintHelperLibrary::GetBlackboard(GetOwner())->SetValueAsBool("Turn", true);
	}
}

void UTurnComponent::SetTurnNum(int TurnNum)
{
	MyTurnNum = TurnNum;
}

void UTurnComponent::UpdateCurMovingAbility(float MovedDist)
{
	CurMovingAbility -= MovedDist;
}

void UTurnComponent::SetCurActionAbility(int value)
{
	CurActionAbility = value;
}

bool UTurnComponent::CanMove(float Distance)
{
	return Distance < CurMovingAbility ? true : false;
}

bool UTurnComponent::CanAttack()
{
	return CurActionAbility < OriginActionAbility ? true : false;
}

