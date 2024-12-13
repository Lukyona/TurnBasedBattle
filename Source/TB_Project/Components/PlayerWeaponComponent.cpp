// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PlayerWeaponComponent.h"
#include "Engine/DataTable.h"

#include "Global.h"
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
	if (!CurrentWeapon) 
	{
		return;
	}

	if (NewType == EWeaponType::UnArmed)
	{
		SetUnArmedMode();
		return;
	}

	if (EquippedWeapon) // 기존에 장비 중인 무기가 있었다면
	{
		EquippedWeapon->UnEquip();
		EquippedWeapon->SetActorHiddenInGame(true);
	}

	EquippedWeapon = CurrentWeapon;
	EquippedWeapon->Equip();
	ChangeWeaponType(NewType);

	if (MAINPC && GetOwner() == MAINPC->GetPawn())
	{
		SetSkillIcons();
	}
}

void UPlayerWeaponComponent::ChangeWeaponType(EWeaponType NewType)
{
	Type = NewType;

	if (OnWeaponTypeChanged.IsBound())
	{
		OnWeaponTypeChanged.Broadcast(NewType);
	}
}

void UPlayerWeaponComponent::SetUnArmedMode()
{
	if (IsUnArmedMode()) 
	{
		return;
	}

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

	if (!OwnerPlayer) 
	{
		return;
	}

	TArray<FPlayerWeaponData*> Datas;
	OwnerPlayer->GetWeaponData()->GetAllRows<FPlayerWeaponData>("", Datas);
	TSubclassOf<APlayerWeapon> NewWeaponClass = WeaponClasses[(int32)NewType];

	AActor* Weapon = UGameplayStatics::GetActorOfClass(GetWorld(), NewWeaponClass);
	if (Weapon)
	{
		CurrentWeapon = Cast<APlayerWeapon>(Weapon);
	}
	else
	{
		SpawnWeapon(Datas, NewWeaponClass, NewType);
	}
	
	if (MAINPC && GetOwner() == MAINPC->GetPawn())
	{
		SetSkillIcons();
	}
}

void UPlayerWeaponComponent::SpawnWeapon(TArray<FPlayerWeaponData*> WeaponData, TSubclassOf<APlayerWeapon> WeaponClass, EWeaponType NewWeaponType)
{
	FTransform Transform;
	for (FPlayerWeaponData* Data : WeaponData)
	{
		// Enum -> FString 변환
		UEnum* EnumType = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
		FString TypeString = EnumType->GetNameStringByValue(static_cast<int64>(NewWeaponType));

		if (TypeString != Data->WeaponName)
		{
			continue;
		}

		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		CurrentWeapon = GetWorld()->SpawnActor<APlayerWeapon>(WeaponClass, Transform, Params);
		CurrentWeapon->SetDatas(Data->EquipMontage, Data->UnEquipMontage, Data->HitMontages, Data->SkillInfoDT, Data->SkillAnimDT);
		CurrentWeapon->SetActorHiddenInGame(true);
	}
}

void UPlayerWeaponComponent::SetSkillIcons()
{
	if (MAINPC) 
	{
		MAINPC->SetSkills(CurrentWeapon->GetSkillInfoDT());
	}
}

void UPlayerWeaponComponent::SpawnProjectile()
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->SpawnProjectile();
	}
}

void UPlayerWeaponComponent::MoveProjectile()
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->Shoot();
	}
}

void UPlayerWeaponComponent::SpawnMagic(EMagicType MagicType)
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->SpawnMagic(MagicType);
	}
}

void UPlayerWeaponComponent::UnEquip()
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->UnEquip();
	}
}

void UPlayerWeaponComponent::GetHit(EHitDirection Direction)
{
	if (!OwnerPlayer || !EquippedWeapon)
	{
		return;
	}

	int32 Index = static_cast<int32>(Direction);
	if (Index >= 0)
	{
		OwnerPlayer->PlayAnimMontage(EquippedWeapon->GetHitMontage(Index));
	}
}
