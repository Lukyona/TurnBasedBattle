// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GameCharacter.h"
#include "Engine/DataTable.h"
#include "Enemy.generated.h"

/**
 * 
 */

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
	FName WeaponSocketName;

};

UCLASS()
class TB_PROJECT_API AEnemy : public AGameCharacter
{
	GENERATED_BODY()
	
protected:
	AEnemy();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 2; // 1 플레이어, 2 적, 255는 중립

	UPROPERTY(VisibleDefaultsOnly)
	class UAIBehaviorComponent* Behavior;

	UPROPERTY()
		UDataTable* EnemyDataTable;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AgroSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float MinAttackDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MinDamage;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxDamage;

	bool bIsCombatMode;
	bool bIsAttacking;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AEnemyWeapon> WeaponClass;

	AEnemyWeapon* Weapon;
	AEnemyWeapon* SecondWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TArray<class UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TSubclassOf<class USoundWave> DeadSound;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;

	void InitializeFromDataTable(const FName& RowName);
	void SetMeshAndAnim(const FString& MeshPath, const FString& AnimClassPath);


public:

	virtual void Attack() override;
	virtual void EndAttack() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnAgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float GetMinDamage() { return MinDamage; }
	float GetMaxDamage() { return MaxDamage; }

	AEnemyWeapon* GetWeapon() { return Weapon; }
	AEnemyWeapon* GetSecondWeapon() { return SecondWeapon; }

	void Dead();
	void EndDead();

	FORCEINLINE float GetMinAttackDistance() { return MinAttackDistance; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }


};
