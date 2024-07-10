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

	float    leftDistance, rightDistance; // ��,�����߰� ������ �Ÿ�
	FRotator leftRotator, rightRotator;

	Trace(LeftSocketName, leftDistance, leftRotator);
	Trace(RightSocketName, rightDistance, rightRotator);

					// �� �� ���� ������
	float offset = FMath::Min(leftDistance, rightDistance);

	// Pelvis�� Z���� �������� ����Ų��. �㸮�� �� ���� ������ ����
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	// X���� ���̷� ���, Foot_L�� Foot_R�� X���� ���� ����Ų��.  �޹��� ���� ��ġ(X��)�� ������ ����, ���� �ö� ���� ����� ��, ���� ���� �ø����� Ÿ���� ������� ��
	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
													        // �������� ���� ��ġ(X��)�� ����� ����, ���� �ö� ���� ������ ��, ���� ���� �ø����� Ÿ���� �������� ��
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -1.f * (rightDistance - offset), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotator, DeltaTime, InterpSpeed);//leftRotator;
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotator, DeltaTime, InterpSpeed);//rightRotator;
}

void UFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	// ���忡���� ���� ��ġ ��������
	FVector  socket = OwnerCharacter->GetMesh()->GetSocketLocation(InName); // InName : ���� Foot_L, Foot_R

	float    z_start = OwnerCharacter->GetActorLocation().Z;  // ĳ������ �߰� ���� (�㸮 �κ�)
						    // ���� X, Y��ǥ
	FVector  start = FVector(socket.X, socket.Y, z_start);    // ������ 
	
					// ĳ������ �߹ٴ� ���� - TraceDistance (�߹ٴں��� TraceDistance��ŭ �Ʒ�)
	float    z_end = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector  end = FVector(socket.X, socket.Y, z_end);       // ����

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, EDrawDebugTrace::ForOneFrame, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	// ���ݰ� ȸ�� �ʱ�ȭ
	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	if (!hitResult.bBlockingHit) return;
	    
	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();  // ��ŭ (����) �÷����ϴ���
	OutDistance = length + OffsetDistance - TraceDistance;               // OffsetDistance : ����ġ

	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z); // rad->deg ���� ��ȯ
	float pitch = UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z) * -1.f; // ���� ������� �� pitch�� -���� ���� ������ �� ����

	OutRotation = FRotator(pitch, 0, roll); // ���� Z�� ȸ���� �ʿ�����Ƿ� 0



}

