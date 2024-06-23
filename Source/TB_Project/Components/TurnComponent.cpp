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

	if (!bMyTurn) return; // 자신의 턴이 아니라면 여기서 리턴

	// 오너 = 자신이 서브 오브젝트로 달린 액터
	if (GetOwner()->IsA<ACPlayer>()) // 오너가 플레이어블 캐릭터라면
	{
		MAINPC->ChangePlayer(Cast<ACPlayer>(GetOwner()));

		// 무기 장착 상태 확인
		UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(GetOwner());
		if (weaponComp && !weaponComp->GetEquippedWeapon()) // 무기가 장착되지 않았다면
			weaponComp->SetMode(weaponComp->GetCurrentWeaponType()); // 무기 장착

		MAINPC->GetBasicUI()->ResetMovingAbilityBarPercent();
		MAINPC->GetBasicUI()->EnableSkillButtons();
		MAINPC->GetBasicUI()->EnableEndTurnButton();
	}
	else if (GetOwner()->IsA<AEnemy>()) // 오너가 적 캐릭터라면
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

