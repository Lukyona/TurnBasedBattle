// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SkillButton.h"
#include "Engine/DataTable.h"

#include "Global.h"
#include "Structs/PlayerSkillAnimData.h"
#include "Character/CPlayer.h"
#include "Components/PlayerWeaponComponent.h"
#include "Weapons/PlayerWeapon.h"

void USkillButton::PrepareSkill()
{
    if (!MAINPC || !MAINPC->IsCombatMode())
    {
        return;
    }

    UDataTable* SkillAnimDT = MAINPC->GetCurPlayer()->GetWeaponComponent()->GetEquippedWeapon()->GetSkillAnimDT();
    if (!SkillAnimDT)
    {
        return;
    }

    FPlayerSkillAnimData* Row = SkillAnimDT->FindRow<FPlayerSkillAnimData>(FName(*SkillName), "PrepareSkill");
    if (!Row)
    {
        return;
    }

    if (bIsAction)
    {
        MAINPC->PlayAction(Row->SkillMontage, bIsNeedTarget);
    }
    else
    {
        MAINPC->PlayAttackPose(Row->SkillMontage);
    }
}

void USkillButton::GetSkillInfo(FString& OutSkillName, FString& OutSkillExplanation, int32& OutMinDamage, int32& OutMaxDamage)
{
    OutSkillName = SkillKoreanName;
    OutSkillExplanation = SkillExplanation;
    OutMinDamage = MinDamage;
    OutMaxDamage = MaxDamage;
}

int32 USkillButton::GetRandomDamage()
{
    return UKismetMathLibrary::RandomIntegerInRange(MinDamage, MaxDamage);
}

void USkillButton::CancelSelection()
{
    bIsSelected = false;
    RemoveSkillHoverEffect();
}
