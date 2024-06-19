// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GameCharacter.h"
#include "CPlayer.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TB_PROJECT_API ACPlayer : public AGameCharacter
{
	GENERATED_BODY()
	
public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UPlayerAnimInstance* AnimInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere)
		class UFeetComponent* FeetComponent;

	FTimerHandle FollowTimerHandle;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	class UDataTable* WeaponDT;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		class UAnimMontage* AttackMontage;


	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;

public:
	virtual void Attack() override;
	virtual void EndAttack() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void FollowingCurPlayer();
	void ClearFollowTimer();

	void SetAttackMontage(UAnimMontage* Montage) { AttackMontage = Montage; }
	UAnimMontage* GetAttackMontage() { return AttackMontage; }

	void ResetSpringArm();

	void Equip();
	virtual void UnEquip();

	void PlayUnEquipMontage(UAnimMontage* Montage);

	void MoveInCombat(FVector Location);
	bool CanAttack();

	void PlayAction();

	void MouseClickLeft();

	USpringArmComponent* GetSpringArm() { return SpringArm; }
	UPlayerWeaponComponent* GetWeaponComponent();
	UDataTable* GetWeaponData() { return WeaponDT; }
	ACharacter* GetCombatTarget() { return CombatTarget; }

	FName GetWeaponBoneName() { return WeaponBoneName; }

	void PossessAIController();
};
