// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Move.generated.h"

/**
 * 
 */

class AEnemy;

UCLASS()
class TB_PROJECT_API UBTTaskNode_Move : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_Move();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float Radius = 500.f;
	FVector LastLocation;
	float StuckTime = 0.0f;

	bool CheckStuckState(float DeltaSeconds, AEnemy* InEnemy);

};
