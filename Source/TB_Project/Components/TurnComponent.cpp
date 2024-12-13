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

	//�ൿ��, �̵��� �ʱ�ȭ
	SetCurActionAbility(0);
	CurMovingAbility = OriginMovingAbility;

	if (!bIsMyTurn || !MAINPC)
	{
		return; // �ڽ��� ���� �ƴ϶�� ���⼭ ����
	}

	// ���� = �ڽ��� ���� ������Ʈ�� �޸� ����
	if (GetOwner()->IsA<ACPlayer>()) // ���ʰ� �÷��̾�� ĳ���Ͷ��
	{
		MAINPC->ChangePlayer(Cast<ACPlayer>(GetOwner()));

		// ���� ���� ���� Ȯ��
		UPlayerWeaponComponent* WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(GetOwner());
		if (WeaponComp && !WeaponComp->GetEquippedWeapon()) // ���Ⱑ �������� �ʾҴٸ�
		{
			WeaponComp->SetMode(WeaponComp->GetCurrentWeaponType()); // ���� ����
		}
		// UI ������Ʈ: �̵� �� ����, ��ų/�� ���� ��ư Ȱ��ȭ
		MAINPC->GetBasicUI()->ResetMovingAbilityBarPercent();
		MAINPC->GetBasicUI()->EnableSkillButtons();
		MAINPC->GetBasicUI()->EnableEndTurnButton();
	}
	else if (GetOwner()->IsA<AEnemy>()) // ���ʰ� �� ĳ���Ͷ��
	{
		// UAIBlueprintHelperLibrary ����Ϸ��� Build.cs�� AIModule �߰��� ��.
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
