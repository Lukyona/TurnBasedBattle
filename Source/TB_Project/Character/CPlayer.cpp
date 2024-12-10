// Fill out your copyRight notice in the Description page of Project Settings.

#include "Character/CPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "AIController.h"

#include "Global.h"
#include "Components/MyMovementComponent.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Components/FeetComponent.h"
#include "Character/Enemy.h"
#include "Character/PlayerAnimInstance.h"
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

    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetRootComponent());
    Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    Tags.Add("Player");
    
}

void ACPlayer::BeginPlay()
{
    Super::BeginPlay();

    AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->Possess(this);
    }

    GetMesh()->HideBoneByName(WeaponBoneName, EPhysBodyOp::PBO_None);

    MovementComponent->SetWalkMode();
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
    if (StateComponent->IsShieldOn()) 
    {
        return 0.f;
    }

    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // ���ݴ��� ���⿡ ���� �ٸ� ��Ÿ�� ���
    {
        FVector EyeLoc;
        FRotator EyeRot;
        GetActorEyesViewPoint(EyeLoc, EyeRot);

        // �÷��̾� ���� �������� ����
        FVector RelativeLocation = (DamageCauser->GetActorLocation() - EyeLoc).GetSafeNormal();

        //�÷��̾��� ���� ����, ���� ���� ���ϱ�
        FVector ForwardVec = UKismetMathLibrary::GetForwardVector(EyeRot).GetSafeNormal();
        FVector RightVec = UKismetMathLibrary::GetRightVector(EyeRot).GetSafeNormal();

        // Dot Product(����) ���, 1: ���Ͱ� ������ ���� ����, 0: ���Ͱ� ���� ����, -1: ���Ͱ� ������ �ݴ� ����.
        float ForwardDot = FVector::DotProduct(RelativeLocation, ForwardVec); // �÷��̾� ���� �������� �󸶳� �������ִ���
        float RightDot = FVector::DotProduct(RelativeLocation, RightVec); // �÷��̾� ���� �������� �󸶳� �������ִ���

        // Atan2(Y,X), X��(����) �������� �󸶳� ȸ���ߴ���, ��� : �ð����, ������, ���� : �ð� �ݴ� ����, ����
        float Result = UKismetMathLibrary::DegAtan2(RightDot, ForwardDot);
        // -180�� ~ 180��
        if (Result > -45.f && Result < 45.f)
        {
            UKismetSystemLibrary::PrintString(this, TEXT("Front Attack"), true, true, FLinearColor::Green, 2.0f);
            WeaponComponent->GetHit(EHitDirection::Front);
        }
        else if (Result >= 45.f && Result <= 135.f)
        {
            UKismetSystemLibrary::PrintString(this, TEXT("Right Attack"), true, true, FLinearColor::Green, 2.0f);

            WeaponComponent->GetHit(EHitDirection::Right);
        }
        else if (Result > 135.f || Result < -135.f)
        {
            UKismetSystemLibrary::PrintString(this, TEXT("Back Attack"), true, true, FLinearColor::Green, 2.0f);

            WeaponComponent->GetHit(EHitDirection::Back);
        }
        else if (Result <= -45.f && Result >= -135.f)
        {
            UKismetSystemLibrary::PrintString(this, TEXT("Left Attack"), true, true, FLinearColor::Green, 2.0f);

            WeaponComponent->GetHit(EHitDirection::Left);
        }
    }
    return 0.0f;
}

void ACPlayer::FollowingCurrentPlayer()
{
    if (MAINPC == nullptr)
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACPlayer::FollowingCurrentPlayer, 0.5f, false);
        return;
    }

    if (MAINPC->IsCombatMode())
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(FollowTimerHandle))
        {
            return;
        }

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
        {
            GetWorld()->GetTimerManager().SetTimer(FollowTimerHandle, this, &ACPlayer::FollowingCurrentPlayer, 1.5f, true);
        }
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
    if (!StateComponent->IsPrepareMode()) 
    {
        return;
    }

    MovementComponent->SetInterp(true);
    StateComponent->SetAttackMode();

    ResetSpringArm();
    MAINPC->SetFixedCamera(true);

    GetMesh()->GetAnimInstance()->Montage_Resume(AttackMontage);
    TurnComponent->SetCurActionAbility(TurnComponent->GetCurActionAbility() + 1);

    if (!TurnComponent->CanAttack())
    {
        MAINPC->DisableSkillButtons();
    }
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
    float attckDist = WeaponComponent->GetEquippedWeapon()->GetMinAttackDistance();
    if (GetDistanceTo(CombatTarget) >= attckDist) // Ÿ���� ������ ���� ��ȿ �Ÿ� ���� ���� ���� �� 
    {
        if (StateComponent->IsPrepareMode())
        {
            MAINPC->GetCombatUI()->ShowMessage(EMessage::TooFarToAttack);
        }
    }
    else
    {
        if (TurnComponent->CanAttack() && StateComponent->IsPrepareMode()) 
        {
            return true;
        }
    }

    return false;
}

void ACPlayer::PlayAction()
{
    StateComponent->SetAttackMode();
    ResetSpringArm();
    TurnComponent->SetCurActionAbility(TurnComponent->GetCurActionAbility() + 1);

    if (!TurnComponent->CanAttack()) 
    {
        MAINPC->DisableSkillButtons();
    }
}

void ACPlayer::MouseClickLeft()
{
    if (!MAINPC) 
    {
        return;
    }

    MAINPC->MouseLeftClick();
}

void ACPlayer::Equip()
{
    if (!WeaponComponent->GetEquippedWeapon()) 
    {
        return;
    }
    CHelpers::AttachTo(WeaponComponent->GetEquippedWeapon(), GetMesh(), "WeaponSocket");
    WeaponComponent->GetEquippedWeapon()->SetActorHiddenInGame(false);

}

void ACPlayer::UnEquip()
{
    if (!WeaponComponent->GetEquippedWeapon()) 
    {
        return;
    }

    WeaponComponent->GetEquippedWeapon()->SetActorHiddenInGame(true);

    if (MAINPC->IsCombatMode()) 
    {
        return;
    }

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
    if (!AIController) 
    {
        return;
    }

    AIController->Possess(this);
}
