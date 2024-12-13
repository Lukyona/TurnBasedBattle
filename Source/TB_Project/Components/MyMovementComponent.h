// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyMovementComponent.generated.h"

class ACharacter;
class ACPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UMyMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMyMovementComponent();
	virtual void BeginPlay() override;

private:
	bool bMove = true;
	bool bInterp;
	bool bCanRotateCamera;

    UPROPERTY(VisibleAnywhere, Category = "Speed")
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Speed")
	float RunSpeed;

	UPROPERTY(VisibleAnywhere)
	ACharacter* OwnerCharacter;
	UPROPERTY(VisibleAnywhere)
	ACPlayer* PlayerCharacter;

public: //Gettets and setters
	bool CanMove() 
	{
		return bMove; 
	}
	void SetMove(bool Value) 
	{
		bMove = Value; 
	}

	bool CanInterp() 
	{
		return bInterp; 
	}
	void SetInterp(bool Value) 
	{
		bInterp = Value; 
	}

	void SetWalkSpeed(float Value) 
	{
		WalkSpeed = Value; 
	}
	void SetRunSpeed(float Value) 
	{
		RunSpeed = Value; 
	}

	//Core methods
	void SetWalkMode();
	void SetRunMode();
	void SetStandMode();

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void HorizontalLook(float Axis);
	void VerticalLook(float Axis);

	void Zoom(float Axis);
	void OnRotateCamera()
	{
		bCanRotateCamera = true;
	}
	void OffRotateCamera()
	{
		bCanRotateCamera = false;
	}

};
