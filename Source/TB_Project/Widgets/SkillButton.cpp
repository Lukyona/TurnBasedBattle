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
    if (!MAINPC->IsCombatMode()) return;

    TArray<FPlayerSkillAnimData*> datas;
    MAINPC->GetCurPlayer()->GetWeaponComponent()->GetEquippedWeapon()->GetSkillAnimDT()->GetAllRows<FPlayerSkillAnimData>("", datas);
    for (FPlayerSkillAnimData* data : datas)
    {
        if (data->SkillName == SkillName)
        {
            if (bIsAction) MAINPC->PlayAction(data->SkillMontage, bNeedTarget);
            else MAINPC->PlayAttackPose(data->SkillMontage);
        }
    }
}

void USkillButton::GetSkillInfo(FString& Skill_Name, FString& Skill_Explanation, int& Min_Dam, int& Max_Dam)
{
    Skill_Name = SkillKoreanName;
    Skill_Explanation = SkillExplanation;
    Min_Dam = MinDamage;
    Max_Dam = MaxDamage;
}

int USkillButton::GetRandomDamage()
{
    int Damage = UKismetMathLibrary::RandomIntegerInRange(MinDamage, MaxDamage);
    return Damage;
}

void USkillButton::CancelSelection()
{
    bSelected = false;
    RemoveSkillHoverEffect();
}
