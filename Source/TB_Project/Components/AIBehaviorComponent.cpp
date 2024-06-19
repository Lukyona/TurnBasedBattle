// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AIBehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UAIBehaviorComponent::UAIBehaviorComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


void UAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UAIBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAIBehaviorComponent::SetCombatMode(bool Value)
{
	Blackboard->SetValueAsBool(CombatModeKey, Value);
}

void UAIBehaviorComponent::SetPatrolLocation(const FVector& Loc)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, Loc);
}

void UAIBehaviorComponent::SetTarget(ACharacter* Target)
{
	Blackboard->SetValueAsObject(TargetKey, Cast<UObject>(Target));
}

void UAIBehaviorComponent::SetMoveDistance(float Distance)
{
	Blackboard->SetValueAsFloat(MoveDistanceKey, Distance);
}

void UAIBehaviorComponent::SetCanAttack(bool Value)
{
	Blackboard->SetValueAsBool(CanAttackKey, Value);
}

void UAIBehaviorComponent::SetTurn(bool Value)
{
	Blackboard->SetValueAsBool(TurnKey, Value);
}

bool UAIBehaviorComponent::GetCombatMode()
{
	return Blackboard->GetValueAsBool(CombatModeKey);

}

FVector UAIBehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}

FVector UAIBehaviorComponent::GetOriginLocation()
{
	return Blackboard->GetValueAsVector(OriginLocationKey);
}

FVector UAIBehaviorComponent::GetMoveLocation()
{
	return Blackboard->GetValueAsVector(MoveLocationKey);
}

ACharacter* UAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}
