// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerWeaponComponent.h"
#include "Engine/DataTable.h"
#include "Global.h"
#include "Structs/PlayerWeaponData.h"
#include "Character/CPlayer.h"
#include "Weapons/PlayerWeapon.h"


UPlayerWeaponComponent::UPlayerWeaponComponent()
{

}

void UPlayerWeaponComponent::BeginPlay()
{
	OwnerPlayer = Cast<ACPlayer>(GetOwner());
}

void UPlayerWeaponComponent::SetMode(EWeaponType NewType)
{
	if (!CurrentWeapon) return;

	if (NewType == EWeaponType::UnArmed)
	{
		SetUnArmedMode();
		return;
	}

	if (EquippedWeapon)
	{
		EquippedWeapon->UnEquip();
		EquippedWeapon->SetActorHiddenInGame(true);
	}

	EquippedWeapon = CurrentWeapon;
	EquippedWeapon->Equip();
	ChangeWeaponType(NewType);

	if (!MAINPC) return;

	if (GetOwner() == MAINPC->GetPawn())
		SetSkillIcons();
}

void UPlayerWeaponComponent::ChangeWeaponType(EWeaponType NewType)
{
	Type = NewType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(NewType);
}

void UPlayerWeaponComponent::SetUnArmedMode()
{
	if (IsUnArmedMode()) return;

	EquippedWeapon = nullptr;

	ChangeWeaponType(EWeaponType::UnArmed);
}

void UPlayerWeaponComponent::SetFistMode()
{
	SetMode(EWeaponType::Fist);
}

void UPlayerWeaponComponent::SetSwordMode()
{
	SetMode(EWeaponType::Sword);
}

void UPlayerWeaponComponent::SetGunMode()
{
	SetMode(EWeaponType::Gun);
}

void UPlayerWeaponComponent::SetBowMode()
{
	SetMode(EWeaponType::Bow);
}

void UPlayerWeaponComponent::SetMagicMode()
{
	SetMode(EWeaponType::Magic);
}

void UPlayerWeaponComponent::SetCurrentWeapon(EWeaponType NewType)
{
	CurWeaponType = NewType;

	if (!OwnerPlayer) return;

	TArray<FPlayerWeaponData*> datas;
	OwnerPlayer->GetWeaponData()->GetAllRows<FPlayerWeaponData>("", datas);
	TSubclassOf<APlayerWeapon> weaponClass = WeaponClasses[(int32)NewType];

	AActor* weapon = UGameplayStatics::GetActorOfClass(GetWorld(), weaponClass);
	if (weapon)
	{
		CurrentWeapon = Cast<APlayerWeapon>(weapon);
	}
	else
	{
		FTransform transform;
		for (FPlayerWeaponData* data : datas)
		{
			// Enum -> FString º¯È¯
			UEnum* enumType = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
			FString typeString = enumType->GetNameStringByValue(static_cast<int64>(NewType));

			if (typeString != data->WeaponName) continue;

			FActorSpawnParameters params;
			params.Owner = GetOwner();
			CurrentWeapon = GetWorld()->SpawnActor<APlayerWeapon>(weaponClass, transform, params);
			CurrentWeapon->SetDatas(data->EquipMontage, data->UnEquipMontage, data->HitMontages, data->SkillInfoDT, data->SkillAnimDT);
			CurrentWeapon->SetActorHiddenInGame(true);
		}
	}
	
	if (!MAINPC) return;

	if (GetOwner() == MAINPC->GetPawn())
		SetSkillIcons();
}

void UPlayerWeaponComponent::SetSkillIcons()
{
	if (!MAINPC) return;

	MAINPC->SetSkills(CurrentWeapon->GetSkillInfoDT());
}

void UPlayerWeaponComponent::SpawnProjectile()
{
	if (!EquippedWeapon) return;

	EquippedWeapon->SpawnProjectile();
}

void UPlayerWeaponComponent::MoveProjectile()
{
	if (!EquippedWeapon) return;

	EquippedWeapon->Shoot();
}

void UPlayerWeaponComponent::SpawnMagic(EMagicType MagicType)
{
	if (!EquippedWeapon) return;

	EquippedWeapon->SpawnMagic(MagicType);
}

void UPlayerWeaponComponent::UnEquip()
{
	if (!EquippedWeapon) return;
	EquippedWeapon->UnEquip();
}

void UPlayerWeaponComponent::GetHit(EHitDirection dir)
{
	if (!OwnerPlayer) return;

	switch (dir)
	{
	case EHitDirection::Front:
		OwnerPlayer->PlayAnimMontage(GetEquippedWeapon()->GetHitMontage(0));
		break;
	case EHitDirection::Right:
		OwnerPlayer->PlayAnimMontage(GetEquippedWeapon()->GetHitMontage(1));
		break;
	case EHitDirection::Back:
		OwnerPlayer->PlayAnimMontage(GetEquippedWeapon()->GetHitMontage(2));
		break;
	case EHitDirection::Left:
		OwnerPlayer->PlayAnimMontage(GetEquippedWeapon()->GetHitMontage(3));
		break;
	}
}
