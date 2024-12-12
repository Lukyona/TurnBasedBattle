// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Character/GameCharacter.h"
#include "Components/PlayerWeaponComponent.h"
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

USTRUCT(BlueprintType)
struct FPlayerStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MovingAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 ActionAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	FString AnimBlueprintPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponBoneName;
};

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
	UDataTable* PlayerDataTable;

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
	UPlayerWeaponComponent* GetWeaponComponent();

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

protected: //Core methods
	void InitializeFromDataTable(const FName& RowName);
private:
	EHitDirection CalculateHitDirection(AActor* DamageCauser);

	void OnMouseLeftClick();
public:
	void Equip();
	virtual void UnEquip();
	void PlayUnEquipMontage(UAnimMontage* Montage);

	void FollowCurrentPlayer();
	void ClearFollowTimer();

	void MoveInCombat(FVector Location);

	bool CanAttack();

	void PlayAction();

	void ResetSpringArm();

	void PossessAIController();
};
