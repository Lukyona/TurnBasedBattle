// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PlayerWeapon.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Character/Enemy.h"

void APlayerWeapon::BeginPlay()
{
    Super::BeginPlay();

    Capsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnComponentBeginOverlap);
}

void APlayerWeapon::ApplyDamageToTarget(ACharacter* OtherCharacter)
{
    AEnemy* Target = Cast<AEnemy>(OtherCharacter);
    if (Target)
    {
        if (Hitted.Find(Target) == -1) // 타겟을 Hitted 배열에서 찾지 못했다면, 중복 방지
        {
            Hitted.AddUnique(Target);

            float damage = MAINPC->GetSkillDamage();
            FDamageEvent dmgEvent;
            Target->TakeDamage(damage, dmgEvent, MAINPC, this);
        }
    }
}

void APlayerWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor->IsA<AEnemy>()) return;
    Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void APlayerWeapon::Equip()
{
    if (!EquipMontage || !OwnerCharacter) return;

    OwnerCharacter->PlayAnimMontage(EquipMontage);
}

void APlayerWeapon::UnEquip()
{
    if (!UnEquipMontage || !OwnerCharacter) return;

    ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
    if (player)
        player->PlayUnEquipMontage(UnEquipMontage);
}

void APlayerWeapon::SetDatas(UAnimMontage* _EquipMontage, UAnimMontage* _UnEquipMontage, TArray<UAnimMontage*> _HitMontages, UDataTable* _SkillInfoDT, UDataTable* _SkillAnimDT)
{
    EquipMontage = _EquipMontage;
    UnEquipMontage = _UnEquipMontage;
    HitMontages = _HitMontages;
    SkillInfoDT = _SkillInfoDT;
    SkillAnimDT = _SkillAnimDT;
}
