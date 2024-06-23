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

	if (!bMyTurn) return; // �ڽ��� ���� �ƴ϶�� ���⼭ ����

	// ���� = �ڽ��� ���� ������Ʈ�� �޸� ����
	if (GetOwner()->IsA<ACPlayer>()) // ���ʰ� �÷��̾�� ĳ���Ͷ��
	{
		MAINPC->ChangePlayer(Cast<ACPlayer>(GetOwner()));

		// ���� ���� ���� Ȯ��
		UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(GetOwner());
		if (weaponComp && !weaponComp->GetEquippedWeapon()) // ���Ⱑ �������� �ʾҴٸ�
			weaponComp->SetMode(weaponComp->GetCurrentWeaponType()); // ���� ����

		MAINPC->GetBasicUI()->ResetMovingAbilityBarPercent();
		MAINPC->GetBasicUI()->EnableSkillButtons();
		MAINPC->GetBasicUI()->EnableEndTurnButton();
	}
	else if (GetOwner()->IsA<AEnemy>()) // ���ʰ� �� ĳ���Ͷ��
	{
		// UAIBlueprintHelperLibrary ����Ϸ��� Build.cs�� AIModule �߰��� ��.
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

