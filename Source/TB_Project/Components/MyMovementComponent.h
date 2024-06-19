// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UMyMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool bMove = true;
	bool bInterp;

    UPROPERTY(VisibleAnywhere, Category = "Speed")
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Speed")
	float RunSpeed;

	UPROPERTY(VisibleAnywhere)
	class ACharacter* OwnerCharacter;
	UPROPERTY(VisibleAnywhere)
	class ACPlayer* PlayerCharacter;

	bool bCanRotateCamera;

public:	
	UMyMovementComponent();
	virtual void BeginPlay() override;

	bool CanMove() { return bMove; }
	void SetMove(bool Value) { bMove = Value; }

	bool CanInterp() { return bInterp; }
	void SetInterp(bool Value) { bInterp = Value; }

	void SetWalkSpeed(float Value) { WalkSpeed = Value; }
	void SetRunSpeed(float Value) { RunSpeed = Value; }

	void SetWalkMode();
	void SetRunMode();
	void SetStandMode();

	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void HorizontalLook(float Axis);
	void VerticalLook(float Axis);

	void Zoom(float Axis);
	void OnRotateCamera();
	void OffRotateCamera();

};
