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
        FSetElementId ElementId = Hitted.Add(Target);
        if (ElementId.IsValidId())
        {
            float Damage = MAINPC->GetSkillDamage();
            FDamageEvent DamageEvent;
            Target->TakeDamage(Damage, DamageEvent, MAINPC, this);
        }
    }
}

void APlayerWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor->IsA<AEnemy>()) 
    {
        return;
    }
    // 부모 클래스의 동작은 오직 적이 오버랩되었을 때만 유효
    Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void APlayerWeapon::Equip()
{
    if (EquipMontage && OwnerCharacter) 
    {
        OwnerCharacter->PlayAnimMontage(EquipMontage);
    }
}

void APlayerWeapon::UnEquip()
{
    if (UnEquipMontage && OwnerCharacter) 
    {
        ACPlayer* Player = Cast<ACPlayer>(OwnerCharacter);
        if (Player)
        {
            Player->PlayUnEquipMontage(UnEquipMontage);
        }
    }
}

void APlayerWeapon::SetDatas(UAnimMontage* InEquipMontage, UAnimMontage* InUnEquipMontage, TArray<UAnimMontage*> InHitMontages, UDataTable* InSkillInfoDT, UDataTable* InSkillAnimDT)
{
    EquipMontage = InEquipMontage;
    UnEquipMontage = InUnEquipMontage;
    HitMontages = InHitMontages;
    SkillInfoDT = InSkillInfoDT;
    SkillAnimDT = InSkillAnimDT;
}
