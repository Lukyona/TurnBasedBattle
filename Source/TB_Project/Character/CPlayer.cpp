// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Components/FeetComponent.h"
#include "Weapons/PlayerWeapon.h"
#include "Widgets/CombatUI.h"


ACPlayer::ACPlayer()
{
    CHelpers::CreateActorComponent<UPlayerWeaponComponent>(this, &WeaponComponent, "WeaponComponent");
    CHelpers::CreateActorComponent<UFeetComponent>(this, &FeetComponent, "FeetComponent");

    GetMesh()->SetRelativeLocation(FVector(0, 0, -90));

    bUseControllerRotationYaw = false;

    SpringArm->bUsePawnControlRotation = true;
    SpringArm->SetRelativeLocation(FVector(0, 0, 140));
    SpringArm->SetRelativeRotation(FRotator(-20, 90, 0));

    UCapsuleComponent* capsule = Cast<UCapsuleComponent>(GetRootComponent());
    capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    Tags.Add("Player");
}

void ACPlayer::BeginPlay()
{
    Super::BeginPlay();

    AIController = Cast<AAIController>(GetController());
    if(AIController) AIController->Possess(this);

    GetMesh()->HideBoneByName(WeaponBoneName, EPhysBodyOp::PBO_None);
    MovementComponent->SetWalkMode();
}

void ACPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", MovementComponent, &UMyMovementComponent::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", MovementComponent, &UMyMovementComponent::MoveRight);
    PlayerInputComponent->BindAxis("HorizontalLook", MovementComponent, &UMyMovementComponent::HorizontalLook);
    PlayerInputComponent->BindAxis("VerticalLook", MovementComponent, &UMyMovementComponent::VerticalLook);

    PlayerInputComponent->BindAxis("Zoom", MovementComponent, &UMyMovementComponent::Zoom);
    PlayerInputComponent->BindAction("RotateCamera", EInputEvent::IE_Pressed, MovementComponent, &UMyMovementComponent::OnRotateCamera);
    PlayerInputComponent->BindAction("RotateCamera", EInputEvent::IE_Released, MovementComponent, &UMyMovementComponent::OffRotateCamera);

    PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, MovementComponent, &UMyMovementComponent::SetRunMode);
    PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, MovementComponent, &UMyMovementComponent::SetWalkMode);

    PlayerInputComponent->BindAction("MouseLeftClick", EInputEvent::IE_Pressed, this, &ACPlayer::MouseClickLeft);

}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (StateComponent->IsShieldOn()) return 0.f;

    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    FVector eyeLoc;
    FRotator eyeRot;
    GetActorEyesViewPoint(eyeLoc, eyeRot);
    FVector loc = DamageCauser->GetActorLocation() - eyeLoc;

    FVector forwardVec = UKismetMathLibrary::GetForwardVector(eyeRot);
    UKismetMathLibrary::Vector_Normalize(forwardVec);

    FVector rightVec = UKismetMathLibrary::GetRightVector(eyeRot);
    UKismetMathLibrary::Vector_Normalize(rightVec);

    float forwardDot = UKismetMathLibrary::Dot_VectorVector(loc, forwardVec);
    float rightDot = UKismetMathLibrary::Dot_VectorVector(loc, rightVec);

    float result = UKismetMathLibrary::DegAtan2(rightDot, forwardDot);
    if (result > -45.f && result < 45.f)
        WeaponComponent->GetHit(EHitDirection::Front);
    else if(result >= 45.f && result <= 135.f)
        WeaponComponent->GetHit(EHitDirection::Right);
    else if (result > 135.f || result < -135.f)
        WeaponComponent->GetHit(EHitDirection::Back);
    else if (result <= -45.f && result >= -135.f)
        WeaponComponent->GetHit(EHitDirection::Left);

    return 0.0f;
}

void ACPlayer::FollowingCurPlayer()
{
    FTimerHandle TimerHandle;
    if(GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    if (MAINPC == nullptr)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACPlayer::FollowingCurPlayer, 0.5f, false);
        return;
    }

    if (MAINPC->IsCombatMode())
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(FollowTimerHandle)) return;

        ClearFollowTimer();
        AIController->StopMovement();
        WeaponComponent->SetMode(WeaponComponent->GetCurrentWeaponType());
    }
    else
    {
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalActor(MAINPC->GetCurPlayer());
        MoveRequest.SetAcceptanceRadius(100.f);
        FNavPathSharedPtr NavPath;

        AIController->MoveTo(MoveRequest, &NavPath);

        if (!GetWorld()->GetTimerManager().IsTimerActive(FollowTimerHandle))
            GetWorld()->GetTimerManager().SetTimer(FollowTimerHandle, this, &ACPlayer::FollowingCurPlayer, 1.5f, true);
    }
}

void ACPlayer::ClearFollowTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(FollowTimerHandle);
}

void ACPlayer::NotifyActorBeginCursorOver()
{
    MAINPC->SpawnTargetCharacterCircle(this);
}

void ACPlayer::NotifyActorEndCursorOver()
{
    MAINPC->DestroyTargetCharacterCircle();

}

void ACPlayer::Attack()
{
    if (!StateComponent->IsPrepareMode()) return;

    MovementComponent->SetInterp(true);
    StateComponent->SetAttackMode();

    ResetSpringArm();
    MAINPC->SetFixedCamera(true);

    GetMesh()->GetAnimInstance()->Montage_Resume(AttackMontage);
    TurnComponent->SetCurActionAbility(TurnComponent->GetCurActionAbility() + 1);

    if (!TurnComponent->CanAttack())
        MAINPC->DisableSkillButtons();
}

void ACPlayer::EndAttack()
{
    MAINPC->CancelSelectedSkill();
    StateComponent->SetWaitMode();
    MovementComponent->SetInterp(false);
    MAINPC->SetFixedCamera(false);
}

void ACPlayer::ResetSpringArm()
{
    SpringArm->SetRelativeLocation(FVector(0, 0, 140));
}

void ACPlayer::MoveInCombat(FVector Location)
{
    StateComponent->SetMovingMode();
    ResetSpringArm();
    MAINPC->SetFixedCamera(true);
    MovementComponent->SetRunMode();
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(MAINPC, Location);
}

bool ACPlayer::CanAttack()
{
    float attckDist = WeaponComponent->GetEquippedWeapon()->GetAttackDistance();
    if (GetDistanceTo(CombatTarget) >= attckDist) // 타겟이 무기의 공격 유효 거리 내에 있지 않을 때 
    {
        if (StateComponent->IsPrepareMode())
        {
            MAINPC->GetCombatUI()->ShowMessage(EMessage::TooFarToAttack);
        }
    }
    else
    {
        if (TurnComponent->CanAttack() && StateComponent->IsPrepareMode()) return true;
    }

    return false;
}

void ACPlayer::PlayAction()
{
    StateComponent->SetAttackMode();
    ResetSpringArm();
    TurnComponent->SetCurActionAbility(TurnComponent->GetCurActionAbility() + 1);

    if (!TurnComponent->CanAttack()) MAINPC->DisableSkillButtons();
}

void ACPlayer::MouseClickLeft()
{
    if (!MAINPC) return;

    MAINPC->MouseLeftClick();
}

void ACPlayer::Equip()
{
    if (!WeaponComponent->GetEquippedWeapon()) return;
    CHelpers::AttachTo(WeaponComponent->GetEquippedWeapon(), GetMesh(), "WeaponSocket");
    WeaponComponent->GetEquippedWeapon()->SetActorHiddenInGame(false);

}

void ACPlayer::UnEquip()
{
    if (!WeaponComponent->GetEquippedWeapon()) return;

    WeaponComponent->GetEquippedWeapon()->SetActorHiddenInGame(true);

    if (MAINPC->IsCombatMode()) return;

    WeaponComponent->SetUnArmedMode();

    MovementComponent->SetMove(true);

    MAINPC->StartFollowingPlayer();
}

void ACPlayer::PlayUnEquipMontage(UAnimMontage* Montage)
{
    AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    AnimInstance->Montage_Play(Montage, -1.f, EMontagePlayReturnType::MontageLength, Montage->GetSectionLength(0), true);
}

UPlayerWeaponComponent* ACPlayer::GetWeaponComponent()
{
    return CHelpers::GetComponent<UPlayerWeaponComponent>(this);
}

void ACPlayer::PossessAIController()
{
    if (!AIController) return;

    AIController->Possess(this);
}

