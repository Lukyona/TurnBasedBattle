// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TurnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "Global.h"
#include "Character/CPlayer.h"
#include "Character/Enemy.h"
#include "Components/PlayerWeaponComponent.h"
#include "Widgets/BasicUI.h"

static const FName TurnKey("Turn");

UTurnComponent::UTurnComponent()
{
}

void UTurnComponent::SetTurn(bool Turn)
{
	bIsMyTurn = Turn;

	//행동력, 이동력 초기화
	SetCurActionAbility(0);
	CurMovingAbility = OriginMovingAbility;

	if (!bIsMyTurn || !MAINPC)
	{
		return; // 자신의 턴이 아니라면 여기서 리턴
	}

	// 오너 = 자신이 서브 오브젝트로 달린 액터
	if (GetOwner()->IsA<ACPlayer>()) // 오너가 플레이어블 캐릭터라면
	{
		MAINPC->ChangePlayer(Cast<ACPlayer>(GetOwner()));

		// 무기 장착 상태 확인
		UPlayerWeaponComponent* WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(GetOwner());
		if (WeaponComp && !WeaponComp->GetEquippedWeapon()) // 무기가 장착되지 않았다면
		{
			WeaponComp->SetMode(WeaponComp->GetCurrentWeaponType()); // 무기 장착
		}
		// UI 업데이트: 이동 바 리셋, 스킬/턴 종료 버튼 활성화
		MAINPC->GetBasicUI()->ResetMovingAbilityBarPercent();
		MAINPC->GetBasicUI()->EnableSkillButtons();
		MAINPC->GetBasicUI()->EnableEndTurnButton();
	}
	else if (GetOwner()->IsA<AEnemy>()) // 오너가 적 캐릭터라면
	{
		// UAIBlueprintHelperLibrary 사용하려면 Build.cs에 AIModule 추가할 것.
		UAIBlueprintHelperLibrary::GetBlackboard(GetOwner())->SetValueAsBool(TurnKey, true);
	}
}

void UTurnComponent::UpdateCurMovingAbility(float MovedDist)
{
	CurMovingAbility -= MovedDist;
}

bool UTurnComponent::CanMove(float Distance) const
{
	return Distance < CurMovingAbility;
}

bool UTurnComponent::CanAttack() const
{
	return CurActionAbility < OriginActionAbility;
}
