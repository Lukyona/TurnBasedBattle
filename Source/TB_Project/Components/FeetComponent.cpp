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

	float    leftOffset, rightOffset; // 왼,오른발과 땅과의 거리
	FRotator leftRotator, rightRotator;
	FVector leftHitLoc, rightHitLoc;
	Trace(LeftSocketName, leftOffset, leftRotator, leftHitLoc);
	Trace(RightSocketName, rightOffset, rightRotator, rightHitLoc);

	float z = UKismetMathLibrary::Abs((rightHitLoc - leftHitLoc).Z);
	
	Data.IKHipOffset = UKismetMathLibrary::SelectFloat(z * -0.5f, 0, z < 50.f);

	Data.LeftEffector.X = UKismetMathLibrary::FInterpTo(Data.LeftEffector.X, (leftOffset - Data.IKHipOffset), DeltaTime, 20.f);
	Data.RightEffector.X = UKismetMathLibrary::FInterpTo(Data.RightEffector.X, -1.f * (rightOffset - Data.IKHipOffset), DeltaTime, 20.f);

					// 둘 중 낮은 값으로
	float offset = FMath::Min(leftOffset, rightOffset);

	// Pelvis는 Z축이 윗방향을 가리킨다. 허리가 더 낮은 쪽으로 보간
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	// X축을 높이로 사용, Foot_L과 Foot_R은 X축이 위를 가리킨다.  왼발은 로컬 위치(X축)가 음수로 시작, 위로 올라갈 수록 양수가 됨, 발을 위로 올리려면 타겟이 양수여야 함
	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftOffset - offset), DeltaTime, InterpSpeed);
													        // 오른발은 로컬 위치(X축)가 양수로 시작, 위로 올라갈 수록 음수가 됨, 발을 위로 올리려면 타겟이 음수여야 함
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -1.f * (rightOffset - offset), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotator, DeltaTime, InterpSpeed);//leftRotator;
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotator, DeltaTime, InterpSpeed);//rightRotator;
}

void UFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation, FVector& OutHitLocation)
{
	// 월드에서의 소켓 위치 가져오기
	FVector  socket = OwnerCharacter->GetMesh()->GetSocketLocation(InName); // InName :  Foot_L, Foot_R

	float    z_start = OwnerCharacter->GetActorLocation().Z;  // 캐릭터의 중간 높이 (허리 부분)
						    // 발의 X, Y좌표
	FVector  start = FVector(socket.X, socket.Y, z_start);    // 시작점 
	
					// 캐릭터의 발바닥 높이 - TraceDistance (발바닥보다 TraceDistance만큼 아래)
	float    z_end = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector  end = FVector(socket.X, socket.Y, z_end);       // 끝점

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
		true, ignores, EDrawDebugTrace::ForOneFrame, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	// 간격과 회전 초기화
	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	if (!hitResult.bBlockingHit) return;
	    
	OutDistance = (hitResult.Location - OwnerCharacter->GetMesh()->GetComponentLocation()).Z;

	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z); // rad->deg 각도 변환
	float pitch = UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z) * -1.f; // 발이 들려있을 때 pitch가 -여야 발이 내려갈 수 있음

	OutRotation = FRotator(pitch, 0, roll); // 발의 Z축 회전은 필요없으므로 0
	OutHitLocation = hitResult.Location;
}

