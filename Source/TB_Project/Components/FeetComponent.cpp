// Fill out your copyRight notice in the Description page of Project Settings.


#include "Components/FeetComponent.h"
#include "Global.h"

const float HipOffsetThreshold = 50.f;

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

	bool bShouldUpdateIK = OwnerCharacter->GetVelocity().Size() > 0.f || OwnerCharacter->IsPlayingRootMotion();
	if (!bShouldUpdateIK)
	{
		return;
	}

	float    LeftOffset, RightOffset;
	FRotator LeftRotator, RightRotator;
	FVector LeftHitLoc, RightHitLoc;
	Trace(LeftSocketName, LeftOffset, LeftRotator, LeftHitLoc);
	Trace(RightSocketName, RightOffset, RightRotator, RightHitLoc);

	// �� �� ������ ���� �� ��� (Z �� ����)
	float Z_Value = UKismetMathLibrary::Abs((RightHitLoc - LeftHitLoc).Z);
	// ���� ���̰� ũ�� �㸮�� ��������(-0.5f�� ���� ��) ���� ���̰� ������ �㸮�� �����˴ϴ�.
	// Z_Value�� �Ӱ谪(HipOffsetThreshold) �̻��� ���, ���� ���� ����(-0.5f)�� ������� �㸮 ��ġ�� �Ʒ��� ���� ������ ����.
	// �Ӱ谪���� ���� ���(��, ���� ������ ���) �㸮 ��ġ�� �������� �ʰ� �״�� ���� (���� 0).
	Data.IKHipOffset = UKismetMathLibrary::SelectFloat(Z_Value * -0.5f, 0, Z_Value < HipOffsetThreshold);

	//�� ��ġ ��������
	Data.LeftEffector.X = UKismetMathLibrary::FInterpTo(Data.LeftEffector.X, (LeftOffset - Data.IKHipOffset), DeltaTime, InterpSpeed);
	Data.RightEffector.X = UKismetMathLibrary::FInterpTo(Data.RightEffector.X, -1.f * (RightOffset - Data.IKHipOffset), DeltaTime, InterpSpeed);

	//�� ȸ������
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, LeftRotator, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, RightRotator, DeltaTime, InterpSpeed);
}

void UFeetComponent::Trace(FName InName, float& OutFootOffset, FRotator& OutRotation, FVector& OutHitLocation)
{
	// ���忡���� ���� ��ġ ��������
	FVector  SocketLoc = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	float    Z_Start = OwnerCharacter->GetActorLocation().Z;  // ĳ������ �߰� ���� (�㸮 �κ�)
						    
	FVector  Start = FVector(SocketLoc.X, SocketLoc.Y, Z_Start);    // ������ 
	
					// ĳ������ �߹ٴ� ���� - TraceDistance (�߹ٴں��� TraceDistance��ŭ �Ʒ�)
	float    Z_End = Start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector  End = FVector(SocketLoc.X, SocketLoc.Y, Z_End);       // ����

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, EDrawDebugTrace::None, HitResult, true, FLinearColor::Green, FLinearColor::Red);

	// ���ݰ� ȸ�� �ʱ�ȭ
	OutFootOffset = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	if (!HitResult.bBlockingHit) 
	{
		return;
	}
																							// OffsetDistance�� ���� ���� �Ϻΰ� �Ĺ����� �� ����
	OutFootOffset = (HitResult.Location - OwnerCharacter->GetMesh()->GetComponentLocation()).Z + OffsetDistance;

	float roll = UKismetMathLibrary::DegAtan2(HitResult.Normal.Y, HitResult.Normal.Z); // rad->deg ���� ��ȯ
	float pitch = UKismetMathLibrary::DegAtan2(HitResult.Normal.X, HitResult.Normal.Z) * -1.f; // ���� ������� �� pitch�� -���� ���� ������ �� ����

	OutRotation = FRotator(pitch, 0, roll); // ���� Z�� ȸ���� �ʿ�����Ƿ� 0
	OutHitLocation = HitResult.Location;
}
