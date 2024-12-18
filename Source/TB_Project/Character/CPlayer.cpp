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
#include "Components/FeetComponent.h"
#include "Character/Enemy.h"
#include "Character/PlayerAnimInstance.h"
#include "Weapons/PlayerWeapon.h"
#include "Widgets/CombatUI.h"

ACPlayer::ACPlayer()
{
    CHelpers::GetAsset<UDataTable>(&PlayerDataTable, "DataTable'/Game/DataTables/PlayerData.PlayerData'");

    CHelpers::CreateActorComponent<UPlayerWeaponComponent>(this, &WeaponComponent, "WeaponComponent");
    CHelpers::CreateActorComponent<UFeetComponent>(this, &FeetComponent, "FeetComponent");

    bUseControllerRotationYaw = false;

    SpringArm->bUsePawnControlRotation = true;
    SpringArm->SetRelativeLocation(FVector(0, 0, 140));
    SpringArm->SetRelativeRotation(FRotator(-20, 90, 0));

    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetRootComponent());
    Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    Tags.Add("Player");
}

void ACPlayer::InitializeFromDataTable(const FName& RowName)
{
    if (!PlayerDataTable)
    {
        return;
    }

    FPlayerData* Row = PlayerDataTable->FindRow<FPlayerData>(RowName, "");
    if (Row)
    {
        SetMeshAndAnim(Row->MeshPath, Row->AnimBlueprintPath);

        MovementComponent->SetWalkSpeed(Row->WalkSpeed);
        MovementComponent->SetRunSpeed(Row->RunSpeed);

        TurnComponent->SetOriginMoveingAbility(Row->MovingAbility);
        TurnComponent->SetOriginActionAbility(Row->ActionAbility);

        HealthComponent->SetHealth(Row->MaxHealth);
        HealthComponent->SetMaxHealth(Row->MaxHealth);

        WeaponBoneName = Row->WeaponBoneName;
    }
}

void ACPlayer::BeginPlay()
{
    Super::BeginPlay();

    AIController = Cast<AAIController>(GetController());
    if (AIController && AIController->GetPawn() != this)
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

    PlayerInputComponent->BindAction("MouseLeftClick", EInputEvent::IE_Pressed, this, &ACPlayer::OnMouseLeftClick);
}

UPlayerWeaponComponent* ACPlayer::GetWeaponComponent()
{
    return CHelpers::GetComponent<UPlayerWeaponComponent>(this);
}

void ACPlayer::ResetSpringArm()
{
    SpringArm->SetRelativeLocation(FVector(0, 0, 140));
}

void ACPlayer::NotifyActorBeginCursorOver()
{
    if(MAINPC->GetCurPlayer()->GetStateComponent()->IsPrepareMode()) // 조작 캐릭터가 스킬 선택했을 경우만
    {
        MAINPC->SpawnTargetCharacterCircle(this);
    }
}

void ACPlayer::NotifyActorEndCursorOver()
{
    MAINPC->DestroyTargetCharacterCircle();
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (StateComponent->IsShieldOn()) 
    {
        return 0.f;
    }

    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // 공격당한 방향에 따라 다른 몽타주 재생
    WeaponComponent->GetHit(CalculateHitDirection(DamageCauser));

    return 0.0f;
}

EHitDirection ACPlayer::CalculateHitDirection(AActor* DamageCauser)
{
    FVector EyeLoc;
    FRotator EyeRot;
    GetActorEyesViewPoint(EyeLoc, EyeRot);

    // 플레이어 기준 공격자의 방향
    FVector RelativeLocation = (DamageCauser->GetActorLocation() - EyeLoc).GetSafeNormal();

    //플레이어의 정면 벡터, 우측 벡터 구하기
    FVector ForwardVec = UKismetMathLibrary::GetForwardVector(EyeRot).GetSafeNormal();
    FVector RightVec = UKismetMathLibrary::GetRightVector(EyeRot).GetSafeNormal();

    // Dot Product(내적) 결과, 1: 벡터가 완전히 같은 방향, 0: 벡터가 서로 수직, -1: 벡터가 완전히 반대 방향.
    // 플레이어 정면/우측 기준으로 얼마나 떨어져있는지
    float ForwardDot = FVector::DotProduct(RelativeLocation, ForwardVec);
    float RightDot = FVector::DotProduct(RelativeLocation, RightVec);

    // Atan2(Y,X), X축(정면) 기준으로 얼마나 회전했는지, 양수 : 시계방향, 오른쪽, 음수 : 시계 반대 방향, 왼쪽
    float Angle = UKismetMathLibrary::DegAtan2(RightDot, ForwardDot);
    // -180도 ~ 180도
    if (Angle > -45.f && Angle < 45.f) 
    {
        return EHitDirection::Front;
    }
    if (Angle >= 45.f && Angle <= 135.f) 
    {
        return EHitDirection::Right;
    }
    if (Angle > 135.f || Angle < -135.f) 
    {
        return EHitDirection::Back;
    }
    return EHitDirection::Left;
}

void ACPlayer::FollowCurrentPlayer()
{
    if (MAINPC == nullptr)
    {
        GetWorld()->GetTimerManager().SetTimer(FollowTimerHandle, this, &ACPlayer::FollowCurrentPlayer, 0.5f, false);
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
            GetWorld()->GetTimerManager().SetTimer(FollowTimerHandle, this, &ACPlayer::FollowCurrentPlayer, 1.5f, true);
        }
    }
}

void ACPlayer::ClearFollowTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(FollowTimerHandle);
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
    float AttckDist = WeaponComponent->GetEquippedWeapon()->GetMinAttackDistance();
    if (GetDistanceTo(CombatTarget) >= AttckDist) // 타겟이 무기의 공격 유효 거리 내에 있지 않을 때 
    {
        if (StateComponent->IsPrepareMode())
        {
            MAINPC->GetCombatUI()->ShowMessage(EMessage::TooFarToAttack);
        }
        return false;
    }

    return TurnComponent->CanAttack() && StateComponent->IsPrepareMode();
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

void ACPlayer::OnMouseLeftClick()
{
    if (!MAINPC) 
    {
        return;
    }

    MAINPC->OnMouseLeftClick();
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

void ACPlayer::PossessAIController()
{
    if (!AIController || AIController->GetPawn() == this)
    {
        return;
    }
    AIController->Possess(this);
}
