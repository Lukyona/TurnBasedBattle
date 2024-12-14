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
    if (!MAINPC)
    {
        return;
    }

    ACharacter* Target = MAINPC->GetCurPlayer()->GetCombatTarget();
    FTransform Transform;
    switch (Type)
    {
        case EMagicType::Shield:
            Transform = Target->GetMesh()->GetSocketTransform("Shield", ERelativeTransformSpace::RTS_Component);
            Magic = GetWorld()->SpawnActor<AActor>(MagicSkills[0], Transform);
            Magic->AttachToActor(Target, FAttachmentTransformRules::KeepRelativeTransform, "Shield");
            CHelpers::GetComponent<UStateComponent>(Target)->SetBuffMode(EBuffType::Shield);
            {
                GetWorld()->GetTimerManager().SetTimer(CheckValidTimer, this, &AMagic::CheckStateValidity, 2.f, true);
            }
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

    SpawnedRound = MAINPC->GetCurrentRound();
}

void AMagic::CheckStateValidity()
{
    if (!MAINPC)
    {
        return;
    }

    int32 ValidRound = 0;
    switch (Type)
    {
        case EMagicType::Shield:
            ValidRound = 2;
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

    if (MAINPC->GetCurrentRound() - SpawnedRound >= ValidRound) // 더 이상 유효하지 않을 때
    {
        TArray<AActor*> AttachedActors;
        Magic->GetAttachedActors(AttachedActors);
        for (AActor* AttachedActor : AttachedActors)
        {
            CHelpers::GetComponent<UStateComponent>(AttachedActor)->SetBuffMode(EBuffType::None);
        }
        Magic->Destroy();

        GetWorld()->GetTimerManager().ClearTimer(CheckValidTimer);
        CheckValidTimer.Invalidate();
    }
}
