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
    ACPlayer* player = Cast<ACPlayer>(GetOwnerCharacter());
    player->GetMesh()->UnHideBoneByName(player->GetWeaponBoneName());

    FTransform transform = player->GetMesh()->GetSocketTransform("arrow_nock");
    Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, transform);

    if (Arrow) 
        Arrow->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "arrow_nock");
}

void ABow::Shoot()
{
    if (!Arrow) return;

    Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    FVector dir = GetOwnerCharacter()->GetCapsuleComponent()->GetForwardVector();
    Arrow->Shoot(dir);
}

void ABow::UnEquip()
{
    if (Arrow)
        Arrow->Destroy();

    Super::UnEquip();
}
