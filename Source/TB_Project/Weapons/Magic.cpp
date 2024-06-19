// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Magic.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/StateComponent.h"

AMagic::AMagic()
{
    AttackDistance = 1800.f;

    TSubclassOf<AActor> skill;
    CHelpers::GetClass<AActor>(&skill, "Blueprint'/Game/AdvancedShieldSystem/Shield.Shield_C'");
    MagicSkills.Add(skill);
}

void AMagic::SpawnMagic(EMagicType MagicType)
{
    Type = MagicType;

    ACharacter* target = MAINPC->GetCurPlayer()->GetCombatTarget();
    AActor* magic;
    FTransform transform;
    switch (Type)
    {
    case EMagicType::Shield:
        transform = target->GetMesh()->GetSocketTransform("Shield", ERelativeTransformSpace::RTS_Component);
        magic = GetWorld()->SpawnActor<AActor>(MagicSkills[0], transform);
        magic->AttachToActor(target, FAttachmentTransformRules::KeepRelativeTransform, "Shield");
        CHelpers::GetComponent<UStateComponent>(target)->SetBuffMode(EBuffType::Shield);
        break;
    case EMagicType::magic1:
        break;
    case EMagicType::magic2:
        break;
    case EMagicType::magic3:
        break;
    case EMagicType::magic4:
        break;
    case EMagicType::None:
        break;
    default:
        break;
    }
}
