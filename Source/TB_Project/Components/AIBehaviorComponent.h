// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIBehaviorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

	class UBlackboardComponent* Blackboard; // EnemyController���� ����

public:	
	UAIBehaviorComponent();

	UPROPERTY(EditAnywhere, Category = "Key")
		FName CombatModeKey = "CombatMode";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName TurnKey = "Turn";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName CanAttackKey = "CanAttack";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName PatrolLocationKey = "PatrolLocation";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName OriginLocationKey = "OriginLocation";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName MoveLocationKey = "MoveLocation";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName MoveDistanceKey = "MoveDistance";

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetBlackboard(UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

	void SetCombatMode(bool Value);
	void SetPatrolLocation(const FVector& Loc);
	void SetTarget(class ACharacter* Target);
	void SetMoveDistance(float Distance);
	void SetCanAttack(bool Value);
	void SetTurn(bool Value);
		
	bool GetCombatMode();
	FVector GetPatrolLocation();
	FVector GetOriginLocation();
	FVector GetMoveLocation();
	ACharacter* GetTarget();
};
