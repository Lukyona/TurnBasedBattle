// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GameCharacter.h"
#include "CPlayer.generated.h"

/**
 * 
 */
class UInputComponent;
class UPlayerAnimInstance;
class UPlayerWeaponComponent;
class UFeetComponent;
class UDataTable;
class UAnimMontage;

UCLASS(BlueprintType)
class TB_PROJECT_API ACPlayer : public AGameCharacter
{
	GENERATED_BODY()
	
public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
public:
	virtual void Attack() override;
	virtual void EndAttack() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY()
	UPlayerAnimInstance* AnimInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPlayerWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere)
	UFeetComponent* FeetComponent;

	FTimerHandle FollowTimerHandle;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UDataTable* WeaponDT;

	UPROPERTY()
	UAnimMontage* AttackMontage;

public: //Getters and setters
	USpringArmComponent* GetSpringArm() 
	{
		return SpringArm; 
	}

	UAnimMontage* GetAttackMontage() 
	{
		return AttackMontage; 
	}
	void SetAttackMontage(UAnimMontage* Montage) 
	{
		AttackMontage = Montage; 
	}

	UPlayerWeaponComponent* GetWeaponComponent();
	UDataTable* GetWeaponData() 
	{
		return WeaponDT; 
	}
	ACharacter* GetCombatTarget() 
	{
		return CombatTarget; 
	}
	FName GetWeaponBoneName() 
	{
		return WeaponBoneName;
	}

public: //Core methods
	void ResetSpringArm();

	void Equip();
	virtual void UnEquip();

	void PlayUnEquipMontage(UAnimMontage* Montage);

	void FollowingCurrentPlayer();
	void ClearFollowTimer();

	void MoveInCombat(FVector Location);
	bool CanAttack();

	void PlayAction();

	void MouseClickLeft();

	void PossessAIController();
};
