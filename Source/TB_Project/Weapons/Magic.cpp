// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Magic.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/StateComponent.h"

AMagic::AMagic()
{
    MinAttackDistance = 1800.f;

    TSubclassOf<AActor> SkillClass;
    CHelpers::GetClass<AActor>(&SkillClass, "Blueprint'/Game/AdvancedShieldSystem/Shield.Shield_C'");
    if(SkillClass)
    {
        MagicSkills.Add(SkillClass);
    }
}

void AMagic::SpawnMagic(EMagicType MagicType)
{
    Type = MagicType;

    ACharacter* Target = MAINPC->GetCurPlayer()->GetCombatTarget();
    AActor* Magic;
    FTransform Transform;
    switch (Type)
    {
        case EMagicType::Shield:
            Transform = Target->GetMesh()->GetSocketTransform("Shield", ERelativeTransformSpace::RTS_Component);
            Magic = GetWorld()->SpawnActor<AActor>(MagicSkills[0], Transform);
            Magic->AttachToActor(Target, FAttachmentTransformRules::KeepRelativeTransform, "Shield");
            CHelpers::GetComponent<UStateComponent>(Target)->SetBuffMode(EBuffType::Shield);
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
