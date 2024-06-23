// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FeetComponent.h"
#include "Global.h"


UFeetComponent::UFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float    leftDistance, rightDistance;
	FRotator leftRotator, rightRotator;

	Trace(LeftSocketName, leftDistance, leftRotator);
	Trace(RightSocketName, rightDistance, rightRotator);

	float offset = FMath::Min(leftDistance, rightDistance);

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);
	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -1.0f * (rightDistance - offset), DeltaTime, InterpSpeed);
	Data.LeftRotation = leftRotator;
	Data.RightRotation = rightRotator;
}

void UFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	FVector  socket = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	float    z_start = OwnerCharacter->GetActorLocation().Z;  // �㸮�κ�
	FVector  start = FVector(socket.X, socket.Y, z_start);    // ������
	
	float    z_end = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector  end = FVector(socket.X, socket.Y, z_end);        // ����

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	// TraceTypeQuery1 : Visibility
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, EDrawDebugTrace::None, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	if (!hitResult.bBlockingHit) return;

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();  //���麸�� �� 50�Ʒ�
	OutDistance = length + OffsetDistance - TraceDistance;               // 5���� +�� �Ǿ�����

	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	float pitch = UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z) * -1.0f;

	OutRotation = FRotator(pitch, 0, roll);
}

