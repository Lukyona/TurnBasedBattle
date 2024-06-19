// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Bow.h"
#include "Components/CapsuleComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Weapons/Arrow.h"

ABow::ABow()
{
    AttackDistance = 1200.f;
    Capsule->SetRelativeScale3D(FVector::ZeroVector);
    Capsule->SetGenerateOverlapEvents(false);
    CHelpers::GetClass<AArrow>(&ArrowClass, "Blueprint'/Game/Weapons/Player/BP_Arrow.BP_Arrow_C'");

}

void ABow::SpawnProjectile()
{
    ACPlayer* player = Cast<ACPlayer>(GetOwnerCharacter());
    GetOwnerCharacter()->GetMesh()->UnHideBoneByName(player->GetWeaponBoneName());

    FTransform transform = OwnerCharacter->GetMesh()->GetSocketTransform("arrow_nock");
    FActorSpawnParameters params;
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, transform, params);

    Arrow->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "arrow_nock");
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
