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

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->bEnableCameraLag = true;

	CHelpers::GetClass<ADamageAmount>(&DamageTextClass, "Blueprint'/Game/UI/BP_DamageAmount.BP_DamageAmount_C'");
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());

}

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CombatTarget || !MovementComponent->CanInterp()) return;

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
	HealthComponent->GetDamage(DamageAmount);

	DamageText = GetWorld()->SpawnActor<ADamageAmount>(DamageTextClass, GetActorTransform());
	DamageText->SetDamage(DamageAmount);

	return 0.0f;
}

void AGameCharacter::SetCombatTarget(ACharacter* Target)
{
	CombatTarget = Target;
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
	if (!Camera) return FVector::ZeroVector;

					// GetWorldLocation() 대신
	return Camera->GetComponentLocation();
}


