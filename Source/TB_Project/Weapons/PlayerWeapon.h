// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "PlayerWeapon.generated.h"

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
	class UDataTable* SkillInfoDT;
	UDataTable* SkillAnimDT;

	class UAnimMontage* EquipMontage;
    UPROPERTY(VisibleAnywhere)
	class UAnimMontage* UnEquipMontage;
	UPROPERTY(VisibleAnywhere)
	TArray<class UAnimMontage*> HitMontages;

	float AttackDistance;

	virtual void BeginPlay() override;

	virtual void ApplyDamageToTarget(ACharacter* OtherCharacter) override;

    UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UDataTable* GetSkillInfoDT() { return SkillInfoDT; }
	UDataTable* GetSkillAnimDT() { return SkillAnimDT; }
	float GetAttackDistance() { return AttackDistance; }

	void Equip();
	virtual void UnEquip();
	void SetDatas(UAnimMontage* _EquipMontage, UAnimMontage* _UnEquipMontage, TArray<UAnimMontage*> _HitMontages, UDataTable* _SkillInfoDT, UDataTable* _SkillAnimDT);

	virtual void SpawnProjectile() {};
	virtual void Shoot() {};
	virtual void SpawnMagic(EMagicType Type) {};


	UAnimMontage* GetHitMontage(int idx) { return HitMontages[idx]; }
};
