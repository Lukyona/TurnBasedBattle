// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Bow.h"
#include "Components/CapsuleComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Weapons/Arrow.h"

ABow::ABow()
{
    MinAttackDistance = 1200.f;

    Capsule->SetRelativeScale3D(FVector::ZeroVector);
    Capsule->SetGenerateOverlapEvents(false);

    CHelpers::GetClass<AArrow>(&ArrowClass, "Blueprint'/Game/Weapons/Player/BP_Arrow.BP_Arrow_C'");
}

void ABow::SpawnProjectile()
{
    ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter());
    if (!Player)
    {
        return;
    }
    Player->GetMesh()->UnHideBoneByName(Player->GetWeaponBoneName());

    FTransform ArrowTransform = Player->GetMesh()->GetSocketTransform("arrow_nock");
    if(ArrowClass)
    {
        Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, ArrowTransform);
        if (Arrow)
        {
            Arrow->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "arrow_nock");
        }
    }
}

void ABow::Shoot()
{
    if (!Arrow) 
    {
        return;
    }

    Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    FVector Direction = GetOwnerCharacter()->GetCapsuleComponent()->GetForwardVector();
    Arrow->Shoot(Direction);
}

void ABow::UnEquip()
{
    if (Arrow)
    {
        Arrow->Destroy();
    }

    Super::UnEquip();
}
