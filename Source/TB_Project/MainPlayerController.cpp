// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <limits>
#include "Global.h"
#include "Character/GameCharacter.h"
#include "Character/CPlayer.h"
#include "Character/Enemy.h"
#include "Components/MyMovementComponent.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Widgets/BasicUI.h"
#include "Widgets/CombatUI.h"
#include "Widgets/TargetingCircle.h"

AMainPlayerController* AMainPlayerController::MainPlayerController = nullptr;

AMainPlayerController::AMainPlayerController()
{
    CHelpers::GetClass<UBasicUI>(&BasicUIClass, "WidgetBlueprint'/Game/UI/BP_BasicUI.BP_BasicUI_C'");
    CHelpers::GetClass<UCombatUI>(&CombatUIClass, "WidgetBlueprint'/Game/UI/BP_CombatUI.BP_CombatUI_C'");
    CHelpers::GetClass<ATargetingCircle>(&TargetingCircleClass, "Blueprint'/Game/UI/TargetingCircle.TargetingCircle_C'");
}

void AMainPlayerController::BeginPlay()
{
    FInputModeGameAndUI inputMode;
    inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    inputMode.SetHideCursorDuringCapture(false);
    SetInputMode(inputMode);

    bShowMouseCursor = true;

    BasicUI = CreateWidget<UBasicUI>(this, BasicUIClass);
    
    if (BasicUI) BasicUI->AddToViewport();

    PlayerCameraManager->ViewPitchMin = -60.f;
    PlayerCameraManager->ViewPitchMax = -20.f;
}

void AMainPlayerController::SetMainPC(APlayerController* Controller)
{
    MainPlayerController = Cast<AMainPlayerController>(Controller);
}

ACPlayer* AMainPlayerController::GetCurPlayer()
{
    return Cast<ACPlayer>(GetPawn());
}

UStateComponent* AMainPlayerController::GetCurPlayerStateComp()
{
    StateComp = CHelpers::GetComponent<UStateComponent>(GetCurPlayer());
    return StateComp;
}

UTurnComponent* AMainPlayerController::GetTurnComp(ACharacter* Char)
{
    TurnComp = CHelpers::GetComponent<UTurnComponent>(Char);
    return TurnComp;
}

void AMainPlayerController::SpawnTargetCharacterCircle(ACharacter* Target)
{
    if (!GetCurPlayerStateComp()->IsPrepareMode()) return;

    FTransform transform;

    FVector location;
    location.X = Target->GetActorLocation().X;
    location.Y = Target->GetActorLocation().Y;
    location.Z = Target->GetActorLocation().Z - Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    transform.SetLocation(location);
    TargetCharacterCircle = GetWorld()->SpawnActor<ATargetingCircle>(TargetingCircleClass, transform);
    if (Target->IsA<ACPlayer>())
        TargetCharacterCircle->SetColorWhite();

}

void AMainPlayerController::DestroyTargetCharacterCircle()
{
    if (TargetCharacterCircle)
        TargetCharacterCircle->Destroy();

    TargetCharacterCircle = nullptr;
}

void AMainPlayerController::ShowEnemyInfo(FString Name, int CurHealth, int MaxHealth)
{
    CombatUI->ShowEnemyInfo(Name, CurHealth, MaxHealth);
}

void AMainPlayerController::HideEnemyInfo()
{
    CombatUI->HideEnemyInfo();
}

void AMainPlayerController::UpdateEnemyHealth(AEnemy* Enemy)
{
    CombatUI->UpdateEnemyHealth(Enemy);
}

void AMainPlayerController::ChangePlayer(ACPlayer* NewPlayer)
{
    if (GetCurPlayerStateComp()->IsMovingMode() || !BasicUI) return;

    BasicUI->ChangePlayer(NewPlayer);
}

void AMainPlayerController::AddCombatCharacter(AGameCharacter* NewCharacter)
{
    FCombatCharacterInfo info;
    info.Name = NewCharacter->GetName();
    info.Character = NewCharacter;
    CombatCharacterInfos.Add(info);
}

void AMainPlayerController::ExcludeCharacterInCombat(ACharacter* Char)
{
    if (!CombatUI)return;

    CombatUI->RemoveCharButton(Char);
    UpdateCombatCharacters(Char);
    DestroyTargetCharacterCircle();
}

void AMainPlayerController::UpdateCombatCharacters(ACharacter* Char) // ���� ĳ���Ͱ� ���ڷ� ����
{
    if (CombatCharacters.Find(Char) == INDEX_NONE) return;
    
    int index = CombatCharacters.Find(Char);
    CombatCharacters.Remove(Char);

    bool combatOver = true;
    for (ACharacter* ch : CombatCharacters)
    {
        if (ch->IsA<AEnemy>())
        {
            combatOver = false;
            break;
        }
    }

    if (combatOver)
    {
        FinishCombat();
        return;
    }

    for (int i = 0; i < CombatCharacters.Num(); ++i)
    {
        if (index > i) continue;
        
        UTurnComponent* turnComp = GetTurnComp(CombatCharacters[i]);
        if (!turnComp) break;

        turnComp->SetTurnNum(turnComp->GetTurnNum() - 1);
    }
}

void AMainPlayerController::PlayAction(UAnimMontage* Anim, bool NeedTarget)
{
    if (NeedTarget)
    {
        PlayAttackPose(Anim);
        return;
    }
        
    GetCurPlayer()->PlayAnimMontage(Anim);
    GetCurPlayer()->PlayAction();
}

void AMainPlayerController::PlayAttackPose(UAnimMontage* Anim)
{
    GetCurPlayer()->PlayAnimMontage(Anim);
    GetCurPlayer()->GetMesh()->GetAnimInstance()->Montage_Pause(Anim);
    GetCurPlayer()->SetAttackMontage(Anim);

    GetCurPlayerStateComp()->SetPrepareMode();
}

void AMainPlayerController::SetSkills(UDataTable* SkillDT)
{
    if (!BasicUI) return;
    BasicUI->ShowSkills(SkillDT);

    UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(GetCurPlayer());
    if (weaponComp->GetCurrentWeaponType() == EWeaponType::Gun) BasicUI->ShowBulletUIBox();
    else BasicUI->HideBulletUIBox();

}

void AMainPlayerController::DisableSkillButtons()
{
    if (!BasicUI) return;
    BasicUI->DisableSkillButtons();
}

void AMainPlayerController::StopAttackPose()
{
    UAnimMontage* AttackMontage = GetCurPlayer()->GetAttackMontage();
    if (!AttackMontage) return;

    GetCurPlayer()->GetMesh()->GetAnimInstance()->Montage_Stop(0.6, AttackMontage);
    GetCurPlayerStateComp()->SetWaitMode();
}

void AMainPlayerController::CheckRemainingDist()
{
    if (!IsCombatMode() || !GetCurPlayerStateComp()->IsMovingMode()) return;

    UTurnComponent* turnComp = GetTurnComp(GetCurPlayer());
    if (!turnComp) return;

    // ������ �Ÿ� ���ϱ�
    float dist = UKismetMathLibrary::Vector_Distance(GetCurPlayer()->GetActorLocation(), ClickedLocation);
    float movedDist = RemainingDist - dist;

    turnComp->UpdateCurMovingAbility(movedDist);

    if (!BasicUI) return;
    BasicUI->SetMovingAbilityBarPercent();
    RemainingDist = dist;
}

void AMainPlayerController::CheckPlayerNotMoving()
{
    bool notMove = UKismetMathLibrary::EqualEqual_VectorVector(GetCurPlayer()->GetActorLocation(), PlayerPrevLocation, 10.f);
    if (notMove) // �������� ���� ���� (���� ��ġ =  ���� ��ġ)
    {
        if (GetCurPlayerStateComp()->IsMovingMode())
        {
            GetCurPlayerStateComp()->SetWaitMode();
            ClearMoveAbilityTimer();
        }
    }
    else // �̵� ���̸� ���� ��ġ ����
    {
        PlayerPrevLocation = GetCurPlayer()->GetActorLocation();
    }
}

void AMainPlayerController::ClearMoveAbilityTimer()
{
    if (TargetSpotCircle) TargetSpotCircle->Destroy();

    TargetSpotCircle = nullptr;

    if (GetWorld()->GetTimerManager().IsTimerActive(TH_CheckDist))
        GetWorld()->GetTimerManager().ClearTimer(TH_CheckDist);

    if (GetWorld()->GetTimerManager().IsTimerActive(TH_CheckMoving))
        GetWorld()->GetTimerManager().ClearTimer(TH_CheckMoving);


    SetFixedCamera(false);
}

void AMainPlayerController::StartCombat()
{
    if (bCombatMode || !BasicUI) return;

    bCombatMode = true;
    for (FCombatCharacterInfo& info : CombatCharacterInfos)
    {
        UMyMovementComponent* moveComp = CHelpers::GetComponent<UMyMovementComponent>(info.Character);
        if (moveComp) moveComp->SetStandMode(); // �̵� �Ұ�, �� �ڸ��� �� �ֱ�

        // ������ ���� ���ϱ�
        int num = 0;
        while (num == 0)
        {
            num = GetRandomNumber(CombatCharacterInfos.Num());
        }
        info.InitiativeNum = num; //������ ���� �ο�
    }

    CombatUI = CreateWidget<UCombatUI>(this, CombatUIClass);
    CombatUI->AddToViewport();
    // ���� UI ǥ��
    //CombatUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    // ������ ���ڰ� ���� �������(������������) ��� UI �߰�
    int index = 1;
    while (index - 1 != CombatCharacterInfos.Num())
    {
        for (int i = 0; i < CombatCharacterInfos.Num(); ++i)
        {
            const FCombatCharacterInfo* charInfo = &CombatCharacterInfos[i];

            if (charInfo->InitiativeNum != index) continue;

            //��� UI �߰�
            CombatUI->AddCharButtons(charInfo->Character, charInfo->Name);
            UStateComponent* stateComp = CHelpers::GetComponent<UStateComponent>(charInfo->Character);
            UTurnComponent* turnComp = GetTurnComp(charInfo->Character);

            if (!stateComp || !turnComp) return;

            stateComp->SetWaitMode();
            turnComp->SetTurnNum(index);


            // �÷��̾� ĳ������ ���
            if (charInfo->Character->IsA<ACPlayer>())
            {
                UMyMovementComponent* moveComp = CHelpers::GetComponent<UMyMovementComponent>(charInfo->Character);
                moveComp->SetMove(false);
                ACharacter* target = FindNearestTarget(charInfo->Character);
                charInfo->Character->SetCombatTarget(target);

                if (charInfo->Character == GetCurPlayer())
                {
                    UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(charInfo->Character);
                    if (weaponComp) weaponComp->SetMode(weaponComp->GetCurrentWeaponType());

                    DisableSkillButtons();
                    BasicUI->DisableEndTurnButton();
                }
            }

            CombatCharacters.Add(charInfo->Character);

            ++index;
        } 
    }

    BasicUI->ShowEndTurnButton();

    float z = GetCurPlayer()->GetActorForwardVector().Z - 90.f;
    FRotator newRot = FRotator(0, z, 0);
    SetControlRotation(newRot);

    SetFixedCamera(true);
    StartTurn(0);
}

int AMainPlayerController::GetRandomNumber(int Max)
{
    int num = UKismetMathLibrary::RandomIntegerInRange(1, Max);
    for (const FCombatCharacterInfo info : CombatCharacterInfos)
    {
        if (info.InitiativeNum == num)
            return 0;
    }
    return num;
}

// ������ ó�� InCurNum�� 0���� ����, ĳ���� �迭 �ε����� 0���� �����ϹǷ�
// ���� ���� ĳ������ ������ ���ڰ� ���ڷ� ����, ù ��° ĳ���ʹ� 1
// InCurNum = 0 ���� ����, InCurNum = 1 ù ��° ĳ������ ���� ����
void AMainPlayerController::StartTurn(int InCurNum) 
{
    // ���� ĳ���� �� ����
    UTurnComponent* turnComp;
    if (InCurNum > 0)
    {
        turnComp = GetTurnComp(CombatCharacters[InCurNum-1]);
        if (turnComp) turnComp->SetTurn(false);

        BasicUI->DisableSkillButtons();  // ��ų��ư ��Ȱ��ȭ
        BasicUI->DisableEndTurnButton(); // �������ư ��Ȱ��ȭ
    }

    int nextTurnNum;
    // ���� �� �ѹ� ����
    // ��� ĳ������ ���� ����->�ٽ� ù ��° ĳ���ͺ��� ����
    if (InCurNum == CombatCharacters.Num()) nextTurnNum = 0; 
    else nextTurnNum = InCurNum;

    // ���� ĳ���� �� Ȱ��ȭ
    turnComp = GetTurnComp(CombatCharacters[nextTurnNum]);
    turnComp->SetTurn(true); 

    CombatUI->UpdateCharButtons(nextTurnNum); // ��� UI ����(Scale) ����
    SetFixedCamera(false); // ī�޶� ���� ����
}

void AMainPlayerController::ActivateMouseEvent()
{
    bEnableMouseOverEvents = true;
    bEnableClickEvents = true;
}

void AMainPlayerController::DeactivateMouseEvent()
{
    bEnableMouseOverEvents = false;
    bEnableClickEvents = false;
}

void AMainPlayerController::MouseLeftClick()
{
    if (!IsCombatMode() || !GetTurnComp(GetCurPlayer())->IsMyTurn()) return;

    FVector start;
    FVector end;
    TArray<AActor*> ignores;
    FHitResult hitResult;
    //���콺 ��ǥ�� ���� ��ǥ�� �ؼ� ��������
    DeprojectMousePositionToWorld(start, end);
    end = start + (end * 3000.f);

    bool hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1,
        false, ignores, EDrawDebugTrace::None, hitResult, true);

    if (!hit) return;

    UPlayerWeaponComponent* weaponComp = GetCurPlayer()->GetWeaponComponent();

    ACharacter* target = Cast<ACharacter>(hitResult.GetActor());
    if (target)
    {
        if (target->IsA<AEnemy>()) // �� Ŭ��
        {
            if (weaponComp->IsMagicMode() && BasicUI->GetSelectedSkillName() == "Shield") return;

            GetCurPlayer()->SetCombatTarget(target);

            if (GetCurPlayer()->CanAttack())
            {
                GetCurPlayer()->Attack();
            }
            return;
        }
        else if (target->IsA<ACPlayer>())
        {
            if (weaponComp->IsMagicMode())
            {
                GetCurPlayer()->SetCombatTarget(target);

                if (GetCurPlayer()->CanAttack())
                {
                    GetCurPlayer()->Attack();
                    DestroyTargetCharacterCircle();
                    DeactivateMouseEvent();
                }
            }
        }
    }

    // �ٴ� Ŭ��
    if (GetCurPlayerStateComp()->IsPrepareMode()) // ���� �غ� ���¿�����
    {
        CombatUI->ShowMessage(EMessage::NoTarget);
        return;
    }

    FNavLocation projectedLoc;
    UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    bool b = navSystem->ProjectPointToNavigation(hitResult.Location, projectedLoc);
    
    if (!b) // AI�� �� �� ���� ��
    {
        CombatUI->ShowMessage(EMessage::WrongSpot);
        return;
    }

    if (GetCurPlayerStateComp()->IsAttackMode()) return;
   
    float dist = UKismetMathLibrary::Vector_Distance(GetCurPlayer()->GetActorLocation(), projectedLoc);

    bool canMove = GetTurnComp(GetCurPlayer())->CanMove(dist);

    if (canMove) // Ŭ���� ��ġ���� (���� �̵�������) �̵� ��������
    {
        FTransform transform;
        transform.SetLocation(projectedLoc.Location);
        transform.SetScale3D(FVector(1.f, 0.4f, 0.4f));

        if (TargetSpotCircle) TargetSpotCircle->Destroy();
        TargetSpotCircle = GetWorld()->SpawnActor<ATargetingCircle>(TargetingCircleClass, transform);
        if (TargetSpotCircle) TargetSpotCircle->SetColorGreen();

        ClickedLocation = projectedLoc.Location;
        RemainingDist = dist;

        GetCurPlayer()->MoveInCombat(ClickedLocation);
        GetWorld()->GetTimerManager().SetTimer(TH_CheckDist, this, &AMainPlayerController::CheckRemainingDist, 0.05f, true);
        PlayerPrevLocation = GetCurPlayer()->GetActorLocation();
        GetWorld()->GetTimerManager().SetTimer(TH_CheckMoving, this, &AMainPlayerController::CheckPlayerNotMoving, 1.f, true);
    }
    else // �̵� �Ұ�
    {
        UMyMovementComponent* moveComp = CHelpers::GetComponent<UMyMovementComponent>(GetCurPlayer());
        moveComp->SetStandMode();
        CombatUI->ShowMessage(EMessage::CannotMove);
    }
}

ACharacter* AMainPlayerController::FindNearestTarget(ACharacter* Self)
{
    AActor* nearestTarget = nullptr;
    TArray<AActor*> targets;

    float nearestDistance = std::numeric_limits<float>::max(); // float �ִ밪���� �ʱ�ȭ

    if (Self->IsA<ACPlayer>())    // ������ �Ʊ��̶�� Ÿ���� ���� ĳ���͵�
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), targets);
    else if (Self->IsA<AEnemy>()) // ������ �����̶�� Ÿ���� �Ʊ� ĳ���͵�
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPlayer::StaticClass(), targets);

    for (AActor* target : targets)
    {
        float dist = UKismetMathLibrary::Vector_Distance(target->GetActorLocation(), Self->GetActorLocation());

        if (dist < nearestDistance) // ���ΰ� ���� ������ �ִ� Ÿ�� ã��
        {
            nearestDistance = dist;
            nearestTarget = target;
        }
    }

    if (nearestTarget) return Cast<ACharacter>(nearestTarget);

    return Cast<ACharacter>(targets[0]);
}

void AMainPlayerController::FinishCombat()
{
    bCombatMode = false;
    CombatUI->RemoveFromViewport();//SetVisibility(ESlateVisibility::Hidden);
    BasicUI->HideEndTurnButton();

    SetFixedCamera(false);
    for (ACharacter* character : CombatCharacters)
    {
        ACPlayer* player = Cast<ACPlayer>(character);
        if (!player) return;

        player->SetCombatTarget(nullptr);

        UStateComponent* stateComp = CHelpers::GetComponent<UStateComponent>(player);
        if (stateComp) stateComp->SetIdleMode();

        UTurnComponent* turnComp = GetTurnComp(player);
        if (turnComp) turnComp->SetTurn(false);

        UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(player);
        if (weaponComp) weaponComp->UnEquip();

        UMyMovementComponent* moveComp = CHelpers::GetComponent<UMyMovementComponent>(player);
        if (moveComp) moveComp->SetWalkMode();
        
        BasicUI->RemoveAllSkillHoverEffect();
    }

    CombatCharacterInfos.Empty();
    CombatCharacters.Empty();

    BasicUI->EnableSkillButtons();
}

void AMainPlayerController::StartFollowingPlayer()
{
    if (!BasicUI) return;

    for (ACPlayer* player : *BasicUI->GetPlayers())
    {
        if (player != GetCurPlayer())
            player->FollowingCurrentPlayer();
    }
}

void AMainPlayerController::CancelSelectedSkill()
{
    if (!BasicUI) return;

    BasicUI->CancelSelectedSkill();
}

void AMainPlayerController::UpdateBulletText(int CurBullet, int MaxBullet)
{
    if (!BasicUI) return;

    BasicUI->UpdateBulletText(CurBullet, MaxBullet);
}

float AMainPlayerController::GetSkillDamage()
{
    if (!BasicUI) return 0.f;

    return BasicUI->GetSkillDamage();
}

FString AMainPlayerController::GetSelectedSkillName()
{
    if (!BasicUI) return "";

    return BasicUI->GetSelectedSkillName();
}

