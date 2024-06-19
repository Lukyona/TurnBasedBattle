// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/Magic.h"
#include "PlayerWeaponComponent.generated.h"

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

private:
	EWeaponType Type = EWeaponType::UnArmed;
	EWeaponType CurWeaponType = EWeaponType::UnArmed;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class APlayerWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class APlayerWeapon* EquippedWeapon;

	class ACPlayer* OwnerPlayer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<APlayerWeapon>> WeaponClasses;

	virtual void BeginPlay() override;

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

    UFUNCTION(BlueprintCallable)
	void SetMode(EWeaponType NewType);

	void ChangeWeaponType(EWeaponType NewType);

	void SetUnArmedMode();
	void SetFistMode();
	void SetSwordMode();
	void SetGunMode();
	void SetBowMode();
	void SetMagicMode();

	bool IsUnArmedMode() { return Type == EWeaponType::UnArmed ? true : false; }
	bool IsMagicMode() { return Type == EWeaponType::Magic ? true : false; }

	void SetCurrentWeapon(EWeaponType NewType);
	EWeaponType GetCurrentWeaponType() { return CurWeaponType; }

	class APlayerWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	void SetSkillIcons();

	void SpawnProjectile();
	void MoveProjectile();

	void SpawnMagic(EMagicType MagicType);

	void UnEquip();

	void GetHit(EHitDirection dir);
};
