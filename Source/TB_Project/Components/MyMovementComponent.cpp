// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Global.h"
#include "Character/CPlayer.h"

constexpr float SpringArmMoveSpeed = 10.f;
constexpr float ZoomStep = -30.f;
constexpr float MinZoomLength = 100.f;
constexpr float MaxZoomLength = 800.f;


UMyMovementComponent::UMyMovementComponent()
{

}

void UMyMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	PlayerCharacter = Cast<ACPlayer>(OwnerCharacter);
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void UMyMovementComponent::SetWalkMode()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void UMyMovementComponent::SetRunMode()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void UMyMovementComponent::SetStandMode()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0.f;
		OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
}

void UMyMovementComponent::MoveForward(float Axis)
{
	if (MAINPC->IsCombatMode())
	{
		if (MAINPC->IsFixedCamera()) 
		{
			return;
		}

		if (Axis != 0.f)
		{
			PlayerCharacter->bUseControllerRotationYaw = true;
		}
		else
		{
			PlayerCharacter->bUseControllerRotationYaw = false;
		}

		PlayerCharacter->GetSpringArm()->AddRelativeLocation(FVector(0, Axis * SpringArmMoveSpeed, 0));
	}
	else
	{
		if (!bMove) 
		{
			return;
		}

		FRotator Rotator = FRotator(0, PlayerCharacter->GetControlRotation().Yaw, 0);
		FVector  Direction = FQuat(Rotator).GetForwardVector();

		PlayerCharacter->AddMovementInput(Direction, Axis);
	}
}

void UMyMovementComponent::MoveRight(float Axis)
{
	if (MAINPC->IsCombatMode())
	{
		if (MAINPC->IsFixedCamera()) 
		{
			return;
		}

		PlayerCharacter->GetSpringArm()->AddRelativeLocation(FVector(Axis * -SpringArmMoveSpeed, 0, 0));
	}
	else
	{
		if (!bMove) 
		{
			return;
		}

		FRotator Rotator = FRotator(0, PlayerCharacter->GetControlRotation().Yaw, 0);
		FVector  Direction = FQuat(Rotator).GetRightVector();

		PlayerCharacter->AddMovementInput(Direction, Axis);
	}
}

void UMyMovementComponent::HorizontalLook(float Axis)
{
	if (!bCanRotateCamera || MAINPC->IsFixedCamera()) 
	{
		return;
	}

	PlayerCharacter->AddControllerYawInput(Axis);
}

void UMyMovementComponent::VerticalLook(float Axis)
{
	if (!bCanRotateCamera || MAINPC->IsFixedCamera()) 
	{
		return;
	}

	PlayerCharacter->AddControllerPitchInput(Axis);
}

void UMyMovementComponent::Zoom(float Axis)
{
	float Value = PlayerCharacter->GetSpringArm()->TargetArmLength + (Axis * ZoomStep);
	PlayerCharacter->GetSpringArm()->TargetArmLength = FMath::Clamp(Value, MinZoomLength, MaxZoomLength); // Clamp로 값을 특정 범위 안에 가둠
}
