// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "AIController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Global.h"
#include "Components/MyMovementComponent.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Widgets/DamageAmount.h"

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성
	{
		CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
		CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

		CHelpers::CreateActorComponent<UMyMovementComponent>(this, &MovementComponent, "MovementComponent");
		CHelpers::CreateActorComponent<UStateComponent>(this, &StateComponent, "StateComponent");
		CHelpers::CreateActorComponent<UTurnComponent>(this, &TurnComponent, "TurnComponent");
		CHelpers::CreateActorComponent<UHealthComponent>(this, &HealthComponent, "HealthComponent");
	}

	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->bEnableCameraLag = true;

	CHelpers::GetClass<ADamageAmount>(&DamageTextClass, "Blueprint'/Game/UI/BP_DamageAmount.BP_DamageAmount_C'");
}

void AGameCharacter::SetMeshAndAnim(const FString& MeshPath, const FString& AnimClassPath)
{
	USkeletalMesh* CharacterMesh = nullptr;
	CHelpers::GetAsset<USkeletalMesh>(&CharacterMesh, *MeshPath);
	if(CharacterMesh)
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh);
	}

	TSubclassOf<UAnimInstance> CharacterAnimInstance = nullptr;
	CHelpers::GetClass<UAnimInstance>(&CharacterAnimInstance, *AnimClassPath);
	if(CharacterAnimInstance)
	{
		GetMesh()->SetAnimClass(CharacterAnimInstance);
	}
}

void AGameCharacter::SetTransform(const FVector& InTranslation, const FVector& InScale3D)
{
	GetMesh()->SetRelativeTransform(FTransform(
		FRotator(0, -90, 0),
		InTranslation,
		InScale3D
	));
}

void AGameCharacter::SetCapsuleSize(float HalfHeight, float Radius)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(HalfHeight);
	GetCapsuleComponent()->SetCapsuleRadius(Radius);
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
}

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CombatTarget || !MovementComponent || !MovementComponent->CanInterp())
	{
		return;
	}

	//타겟쪽으로 회전
	FVector MyLoc = GetActorLocation();
	FVector TargetLoc = CombatTarget->GetActorLocation();
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyLoc, TargetLoc);
	FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 85.f);
	SetActorRotation(NewRot);
}

void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HealthComponent)
	{
		return 0.0f;
	}
	HealthComponent->GetDamage(DamageAmount);

	DamageText = GetWorld()->SpawnActor<ADamageAmount>(DamageTextClass, GetActorTransform());
	if(DamageText)
	{
		DamageText->SetDamage(DamageAmount);
	}

	return 0.0f;
}

UStateComponent* AGameCharacter::GetStateComponent()
{
	return CHelpers::GetComponent<UStateComponent>(this);
}

UTurnComponent* AGameCharacter::GetTurnComponent()
{
	return CHelpers::GetComponent<UTurnComponent>(this);
}

FVector AGameCharacter::GetCameraWorldLocation()
{
	if (!Camera) 
	{
		return FVector::ZeroVector;
	}

					// GetWorldLocation() 대신
	return Camera->GetComponentLocation();
}
