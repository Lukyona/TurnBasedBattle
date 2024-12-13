// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
class AEnemy;
class UBehaviorTreeComponent;
class UAIBehaviorComponent;
class UMyMovementComponent;
class UStateComponent;
class UTurnComponent;

namespace BTHelper
{
	bool ValidateAllEntities(UBehaviorTreeComponent& OwnerComp, AEnemyController*& OutEnemyController, AEnemy*& OutEnemy,
		UAIBehaviorComponent** OutBehaviorComp = nullptr, UMyMovementComponent** OutMovementComp = nullptr,
		UStateComponent** OutStateComp = nullptr, UTurnComponent** OutTurnComp = nullptr);
}

UCLASS()
class TB_PROJECT_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();
protected:
	virtual void BeginPlay() override;

	class UAIBehaviorComponent* Behavior;
};
