// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_CalculateDistance.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API UBTTaskNode_CalculateDistance : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_CalculateDistance();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
