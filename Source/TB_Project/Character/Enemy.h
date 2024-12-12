// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GameCharacter.h"
#include "Engine/DataTable.h"
#include "Enemy.generated.h"

/**
 * 
 */
class UBehaviorTree;
class UAIBehaviorComponent;
class USphereComponent;
class AEnemyWeapon;
class UAnimMontage;
class USoundWave;

USTRUCT(BlueprintType)
struct FEnemyStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MinAttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxDamage;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	FString WeaponClassPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* DeadMontage;
};

UCLASS()
class TB_PROJECT_API AEnemy : public AGameCharacter
{
	GENERATED_BODY()
	
protected:
	AEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
public:
	virtual void Attack() override;
	virtual void EndAttack() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY()
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleDefaultsOnly)
	UAIBehaviorComponent* Behavior;

	UPROPERTY()
	UDataTable* EnemyDataTable;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* AgroSphere;

	float MinAttackDistance;
	float MinDamage;
	float MaxDamage;

	bool bIsCombatMode;
	bool bIsAttacking;

	TSubclassOf<AEnemyWeapon> WeaponClass;
	AEnemyWeapon* Weapon;
	AEnemyWeapon* SecondWeapon = nullptr;
	FName WeaponSocketName;

	TArray<UAnimMontage*> AttackMontages;
	UAnimMontage* HitMontage;
	UAnimMontage* DeadMontage;

public://Getters and setters
	float GetMinDamage() 
	{
		return MinDamage; 
	}
	float GetMaxDamage() 
	{
		return MaxDamage; 
	}
	float GetMinAttackDistance()
	{
		return MinAttackDistance;
	}

	UBehaviorTree* GetBehaviorTree()
	{
		return BehaviorTree;
	}

	AEnemyWeapon* GetWeapon() 
	{
		return Weapon; 
	}
	AEnemyWeapon* GetSecondWeapon() 
	{
		return SecondWeapon; 
	}

protected: //Core methods
	void InitializeFromDataTable(const FName& RowName);

	UFUNCTION()
	void OnAgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void Dead();
public:
	void EndDead();
};
