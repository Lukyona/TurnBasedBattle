// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/PlayerWeaponData.h"
#include "Weapons/Magic.h"
#include "PlayerWeaponComponent.generated.h"

class APlayerWeapon;
class ACPlayer;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	UnArmed, Fist, Sword, Gun, Bow, Magic,
};

enum class EHitDirection : uint8
{
	Front, Right, Back, Left,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponTypeChanged, EWeaponType, NewWeapon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UPlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerWeaponComponent();
protected:
	virtual void BeginPlay() override;

private:
	EWeaponType Type = EWeaponType::UnArmed;
	EWeaponType CurWeaponType = EWeaponType::UnArmed;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	APlayerWeapon* CurrentWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	APlayerWeapon* EquippedWeapon;

	ACPlayer* OwnerPlayer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<APlayerWeapon>> WeaponClasses;

public:// Getters and setters
	bool IsUnArmedMode()
	{
		return Type == EWeaponType::UnArmed;
	}
	bool IsMagicMode()
	{
		return Type == EWeaponType::Magic;
	}

	EWeaponType GetCurrentWeaponType()
	{
		return CurWeaponType;
	}

	APlayerWeapon* GetEquippedWeapon()
	{
		return EquippedWeapon;
	}

	void SetUnArmedMode();
	void SetFistMode();
	void SetSwordMode();
	void SetGunMode();
	void SetBowMode();
	void SetMagicMode();

	//Core methods
    UFUNCTION(BlueprintCallable)
	void SetMode(EWeaponType NewType);

	void ChangeWeaponType(EWeaponType NewType);

	void SetCurrentWeapon(EWeaponType NewType);

	void SetSkillIcons();

	void SpawnProjectile();
	void MoveProjectile();

	void SpawnMagic(EMagicType MagicType);

	void SpawnWeapon(TArray<FPlayerWeaponData*> WeaponData, TSubclassOf<APlayerWeapon> WeaponClass, EWeaponType NewWeaponType);

	void UnEquip();

	void GetHit(EHitDirection Direction);

	FWeaponTypeChanged OnWeaponTypeChanged;

};
