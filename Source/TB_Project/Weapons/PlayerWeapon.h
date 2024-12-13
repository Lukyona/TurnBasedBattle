// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "PlayerWeapon.generated.h"

class UDataTable;
class UAnimMontage;

UENUM(BlueprintType)
enum class EMagicType : uint8
{
	Shield, magic1, magic2, magic3, magic4, None,
};

UCLASS()
class TB_PROJECT_API APlayerWeapon : public AWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void ApplyDamageToTarget(ACharacter* OtherCharacter) override;

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY()
	UDataTable* SkillInfoDT;
	UPROPERTY()
	UDataTable* SkillAnimDT;

	UPROPERTY()
	UAnimMontage* EquipMontage;
    UPROPERTY()
	UAnimMontage* UnEquipMontage;
	UPROPERTY()
	TArray<UAnimMontage*> HitMontages;

	float MinAttackDistance;

public://Getters and setters
	UDataTable* GetSkillInfoDT() 
	{
		return SkillInfoDT; 
	}
	UDataTable* GetSkillAnimDT() 
	{
		return SkillAnimDT; 
	}
	float GetMinAttackDistance() 
	{
		return MinAttackDistance; 
	}
	UAnimMontage* GetHitMontage(int32 Idx) 
	{
		return HitMontages[Idx]; 
	}

	//Core methods
	void Equip();
	virtual void UnEquip();

	void SetDatas(UAnimMontage* _EquipMontage, UAnimMontage* _UnEquipMontage, TArray<UAnimMontage*> _HitMontages, UDataTable* _SkillInfoDT, UDataTable* _SkillAnimDT);

	virtual void SpawnProjectile() {};
	virtual void Shoot() {};
	virtual void SpawnMagic(EMagicType Type) {};

};
