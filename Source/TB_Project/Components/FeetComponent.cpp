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

	// 양 발 사이의 높이 차 계산 (Z 축 기준)
	float Z_Value = UKismetMathLibrary::Abs((RightHitLoc - LeftHitLoc).Z);
	// 높이 차이가 크면 허리가 내려가고(-0.5f로 음수 값) 높이 차이가 작으면 허리가 유지됩니다.
	// Z_Value가 임계값(HipOffsetThreshold) 이상일 경우, 높이 차의 절반(-0.5f)을 기반으로 허리 위치를 아래로 내려 균형을 유지.
	// 임계값보다 낮을 경우(즉, 거의 평지일 경우) 허리 위치는 보정하지 않고 그대로 유지 (값은 0).
	Data.IKHipOffset = UKismetMathLibrary::SelectFloat(Z_Value * -0.5f, 0, Z_Value < HipOffsetThreshold);

	//발 위치 선형보간
	Data.LeftEffector.X = UKismetMathLibrary::FInterpTo(Data.LeftEffector.X, (LeftOffset - Data.IKHipOffset), DeltaTime, InterpSpeed);
	Data.RightEffector.X = UKismetMathLibrary::FInterpTo(Data.RightEffector.X, -1.f * (RightOffset - Data.IKHipOffset), DeltaTime, InterpSpeed);

	//발 회전보간
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, LeftRotator, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, RightRotator, DeltaTime, InterpSpeed);
}

void UFeetComponent::Trace(FName InName, float& OutFootOffset, FRotator& OutRotation, FVector& OutHitLocation)
{
	// 월드에서의 소켓 위치 가져오기
	FVector  SocketLoc = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	float    Z_Start = OwnerCharacter->GetActorLocation().Z;  // 캐릭터의 중간 높이 (허리 부분)
						    
	FVector  Start = FVector(SocketLoc.X, SocketLoc.Y, Z_Start);    // 시작점 
	
					// 캐릭터의 발바닥 높이 - TraceDistance (발바닥보다 TraceDistance만큼 아래)
	float    Z_End = Start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector  End = FVector(SocketLoc.X, SocketLoc.Y, Z_End);       // 끝점

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, EDrawDebugTrace::None, HitResult, true, FLinearColor::Green, FLinearColor::Red);

	// 간격과 회전 초기화
	OutFootOffset = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	if (!HitResult.bBlockingHit) 
	{
		return;
	}
																							// OffsetDistance를 더해 발의 일부가 파묻히는 걸 방지
	OutFootOffset = (HitResult.Location - OwnerCharacter->GetMesh()->GetComponentLocation()).Z + OffsetDistance;

	float roll = UKismetMathLibrary::DegAtan2(HitResult.Normal.Y, HitResult.Normal.Z); // rad->deg 각도 변환
	float pitch = UKismetMathLibrary::DegAtan2(HitResult.Normal.X, HitResult.Normal.Z) * -1.f; // 발이 들려있을 때 pitch가 -여야 발이 내려갈 수 있음

	OutRotation = FRotator(pitch, 0, roll); // 발의 Z축 회전은 필요없으므로 0
	OutHitLocation = HitResult.Location;
}
