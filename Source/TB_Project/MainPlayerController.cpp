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

constexpr float MinCameraPitch = -60.f;
constexpr float MaxCameraPitch = -20.f;
constexpr float PlayerStopThreshold = 10.f;
constexpr float MaxTraceLength = 3000.f;

AMainPlayerController* AMainPlayerController::MainPlayerController = nullptr;

AMainPlayerController::AMainPlayerController()
{
    CHelpers::GetClass<UBasicUI>(&BasicUIClass, "WidgetBlueprint'/Game/UI/BP_BasicUI.BP_BasicUI_C'");
    CHelpers::GetClass<UCombatUI>(&CombatUIClass, "WidgetBlueprint'/Game/UI/BP_CombatUI.BP_CombatUI_C'");
    CHelpers::GetClass<ATargetingCircle>(&TargetingCircleClass, "Blueprint'/Game/UI/TargetingCircle.TargetingCircle_C'");
}

void AMainPlayerController::BeginPlay()
{
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

    bShowMouseCursor = true;

    BasicUI = CreateWidget<UBasicUI>(this, BasicUIClass);
    if (BasicUI) 
    {
        BasicUI->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create BasicUI!"));
        checkf(BasicUI, TEXT("BasicUI creation failed! Game cannot continue.")); //false일 경우 실행 중단
        return;
    }

    PlayerCameraManager->ViewPitchMin = MinCameraPitch;
    PlayerCameraManager->ViewPitchMax = -MaxCameraPitch;
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
    return CHelpers::GetComponent<UStateComponent>(GetCurPlayer());
}

UTurnComponent* AMainPlayerController::GetTurnComp(ACharacter* InCharacter)
{
    return CHelpers::GetComponent<UTurnComponent>(InCharacter);
}

void AMainPlayerController::SpawnTargetCharacterCircle(ACharacter* Target)
{
    if (!GetCurPlayerStateComp()->IsPrepareMode()) // 공격 준비 상태일 때만 실행
    {
        return;
    }

    FVector TargetLocation;
    TargetLocation.X = Target->GetActorLocation().X;
    TargetLocation.Y = Target->GetActorLocation().Y;
    TargetLocation.Z = Target->GetActorLocation().Z - Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    FTransform TargetTransform;
    TargetTransform.SetLocation(TargetLocation);
    TargetCharacterCircle = GetWorld()->SpawnActor<ATargetingCircle>(TargetingCircleClass, TargetTransform);

    if (TargetCharacterCircle && Target->IsA<ACPlayer>())
    {
        TargetCharacterCircle->SetColorWhite();
    }
}

void AMainPlayerController::DestroyTargetCharacterCircle()
{
    if (TargetCharacterCircle)
    {
        TargetCharacterCircle->Destroy();
    }
    TargetCharacterCircle = nullptr;
}

void AMainPlayerController::ShowEnemyInfo(FString Name, int32 CurHealth, int32 MaxHealth)
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
    if (GetCurPlayerStateComp()->IsMovingMode()) 
    {
        return;
    }

    BasicUI->ChangePlayer(NewPlayer);
}

void AMainPlayerController::AddCombatCharacter(AGameCharacter* NewCharacter)
{
    FCombatCharacterInfo CharacterInfo;
    CharacterInfo.Name = NewCharacter->GetName();
    CharacterInfo.Character = NewCharacter;
    CombatCharacterInfos.Add(CharacterInfo);
}

void AMainPlayerController::ExcludeCharacterInCombat(ACharacter* DeadCharacter)
{
    if (!CombatUI)
    {
        return;
    }
    DestroyTargetCharacterCircle();
    CombatUI->RemoveCharButton(DeadCharacter);
    UpdateCombatCharacters(DeadCharacter);
}

void AMainPlayerController::UpdateCombatCharacters(ACharacter* DeadCharacter) // 죽은 캐릭터가 인자로 들어옴
{
    if (!CombatCharacters.Contains(DeadCharacter)) 
    {
        return;
    }
    
    // 전투 참여 캐릭터 배열에서 제거
    int32 CharacterIndex = CombatCharacters.Find(DeadCharacter);
    CombatCharacters.Remove(DeadCharacter);

    // 살아있는 적군이 있는지 확인
    bool bIsCombatOver = true;
    for (ACharacter* CombatCharacter : CombatCharacters)
    {
        if (CombatCharacter->IsA<AEnemy>())
        {
            bIsCombatOver = false;
            break;
        }
    }

    if (bIsCombatOver)
    {
        FinishCombat();
        return;
    }

    for (int32 i = 0; i < CombatCharacters.Num(); ++i)
    {
        if (CharacterIndex > i) 
        {
            continue;
        }
        
        // 순서 갱신
        TurnComp = GetTurnComp(CombatCharacters[i]);
        if (TurnComp) 
        {
            TurnComp->SetTurnNum(TurnComp->GetTurnNum() - 1);
        }
    }
}

void AMainPlayerController::PlayAction(UAnimMontage* Anim, bool bIsNeedTarget)
{
    if (bIsNeedTarget)
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

void AMainPlayerController::ShowSkills(UDataTable* SkillDT)
{
    BasicUI->ShowSkills(SkillDT);

    WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(GetCurPlayer());
    if (WeaponComp && WeaponComp->GetCurrentWeaponType() == EWeaponType::Gun)
    {
        BasicUI->ShowBulletUIBox();
    }
    else 
    {
        BasicUI->HideBulletUIBox();
    }
}

void AMainPlayerController::DisableSkillButtons()
{
    BasicUI->DisableSkillButtons();
}

void AMainPlayerController::StopAttackPose()
{
    UAnimMontage* AttackMontage = GetCurPlayer()->GetAttackMontage();
    if (!AttackMontage) 
    {
        return;
    }

    GetCurPlayer()->GetMesh()->GetAnimInstance()->Montage_Stop(0.6, AttackMontage);
    GetCurPlayerStateComp()->SetWaitMode();
}

void AMainPlayerController::UpdateMovingAbility()
{
    if (!IsCombatMode() || !GetCurPlayerStateComp()->IsMovingMode())
    {
        return;
    }

    TurnComp = GetTurnComp(GetCurPlayer());
    if (!TurnComp) 
    {
        return;
    }

    // 움직인 거리 구하기
    float DistanceToTarget = UKismetMathLibrary::Vector_Distance(GetCurPlayer()->GetActorLocation(), ClickedLocation);
    float MovedDist = RemainingDist - DistanceToTarget;

    TurnComp->UpdateCurMovingAbility(MovedDist);

    BasicUI->SetMovingAbilityBarPercent();
    RemainingDist = DistanceToTarget;
}

void AMainPlayerController::CheckPlayerMovingState()
{
    bool bIsPlayerStopped = UKismetMathLibrary::EqualEqual_VectorVector(GetCurPlayer()->GetActorLocation(), PlayerLastLocation, PlayerStopThreshold);
    if (bIsPlayerStopped) // 움직이지 않은 상태 (기존 위치 =  현재 위치)
    {
        if (GetCurPlayerStateComp()->IsMovingMode())
        {
            GetCurPlayerStateComp()->SetWaitMode();
            ClearMoveAbilityTimer();
        }
    }
    else // 이동 중이면 기존 위치 갱신
    {
        PlayerLastLocation = GetCurPlayer()->GetActorLocation();
    }
}

void AMainPlayerController::ClearMoveAbilityTimer()
{
    if (TargetSpotCircle) 
    {
        TargetSpotCircle->Destroy();
    }

    TargetSpotCircle = nullptr;

    if (GetWorld()->GetTimerManager().IsTimerActive(CheckDistTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(CheckDistTimer);
    }

    if (GetWorld()->GetTimerManager().IsTimerActive(CheckMovingTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(CheckMovingTimer);
    }

    SetFixedCamera(false);
}

void AMainPlayerController::StartCombat()
{
    if (bIsCombatMode) 
    {
        return;
    }

    bIsCombatMode = true;
    for (FCombatCharacterInfo& CharacterInfo : CombatCharacterInfos)
    {
        MoveComp = CHelpers::GetComponent<UMyMovementComponent>(CharacterInfo.Character);
        if (MoveComp) 
        {
            MoveComp->SetStandMode(); // 이동 불가, 그 자리에 서 있기
        }

        // 선제권 숫자 정하기
        int32 Num = 0;
        while (Num == 0)
        {
            Num = GetRandomNumber(CombatCharacterInfos.Num());
        }
        CharacterInfo.InitiativeNum = Num; //선제권 숫자 부여
    }

    CombatUI = CreateWidget<UCombatUI>(this, CombatUIClass);
    CombatUI->AddToViewport();

    // 선제권 숫자 기준 오름차순으로 정렬
    CombatCharacterInfos.Sort([](const FCombatCharacterInfo& A, const FCombatCharacterInfo& B)
    {
        return A.InitiativeNum < B.InitiativeNum;
    });

    // 선제권 숫자 기준 오름차순으로 상단 UI 추가
    for (const FCombatCharacterInfo& CharacterInfo : CombatCharacterInfos)
    {
        CombatUI->AddCharButtons(CharacterInfo.Character, CharacterInfo.Name);
        StateComp = CHelpers::GetComponent<UStateComponent>(CharacterInfo.Character);
        TurnComp = GetTurnComp(CharacterInfo.Character);
        if (!StateComp || !TurnComp) 
        {
            return;
        }

        StateComp->SetWaitMode();
        TurnComp->SetTurnNum(CharacterInfo.InitiativeNum);

        // 플레이어 캐릭터일 경우
        if (CharacterInfo.Character->IsA<ACPlayer>())
        {
            MoveComp = CHelpers::GetComponent<UMyMovementComponent>(CharacterInfo.Character);
            if(MoveComp)
            {
                MoveComp->SetMove(false);
            }

            ACharacter* Target = FindNearestTarget(CharacterInfo.Character);
            if(Target)
            {
                CharacterInfo.Character->SetCombatTarget(Target);
            }

            if (CharacterInfo.Character == GetCurPlayer())
            {
                WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(CharacterInfo.Character);
                if (WeaponComp) 
                {
                    WeaponComp->SetMode(WeaponComp->GetCurrentWeaponType());
                }

                DisableSkillButtons();
                BasicUI->DisableEndTurnButton();
            }
        }
        CombatCharacters.Add(CharacterInfo.Character);
    }

    BasicUI->ShowEndTurnButton();

    bEnableMouseOverEvents = true;

    float ZValue = GetCurPlayer()->GetActorForwardVector().Z - 90.f;
    FRotator NewRot = FRotator(0, ZValue, 0);
    SetControlRotation(NewRot);

    SetFixedCamera(true);
    StartTurn(0);
}

int32 AMainPlayerController::GetRandomNumber(int32 Max)
{
    int32 Num = UKismetMathLibrary::RandomIntegerInRange(1, Max);
    for (const FCombatCharacterInfo CharInfo : CombatCharacterInfos)
    {
        if (CharInfo.InitiativeNum == Num)
        {
            return 0;
        }
    }
    return Num;
}

// 전투의 처음 InCurNum은 0부터 시작, 캐릭터 배열 인덱스가 0부터 시작하므로
// 턴이 끝난 캐릭터의 선제권 숫자가 인자로 들어옴, 첫 번째 캐릭터는 1
// InCurNum = 0 전투 시작, InCurNum = 1 첫 번째 캐릭터의 턴이 끝남
void AMainPlayerController::StartTurn(int32 InCurNum) 
{
    // 이전 캐릭터 턴 해제
    if (InCurNum > 0)
    {
        TurnComp = GetTurnComp(CombatCharacters[InCurNum-1]);
        if (TurnComp) 
        {
            TurnComp->SetTurn(false);
        }

        BasicUI->DisableSkillButtons();  // 스킬버튼 비활성화
        BasicUI->DisableEndTurnButton(); // 턴종료버튼 비활성화
    }

    int32 NextTurnNum;
    // 다음 턴 넘버 갱신
    // 모든 캐릭터의 턴이 끝남->다시 첫 번째 캐릭터부터 시작
    if (InCurNum == CombatCharacters.Num()) 
    {
        NextTurnNum = 0;
    }
    else 
    {
        NextTurnNum = InCurNum;
    }

    // 다음 캐릭터 턴 활성화
    TurnComp = GetTurnComp(CombatCharacters[NextTurnNum]);
    TurnComp->SetTurn(true);

    CombatUI->UpdateCharButtons(NextTurnNum); // 상단 UI 상태(Scale) 갱신
    SetFixedCamera(false); // 카메라 고정 해제
}

void AMainPlayerController::EnableMouseClickEvent()
{
    bEnableClickEvents = true;
}

void AMainPlayerController::DisableMouseClickEvent()
{
    bEnableClickEvents = false;
}

void AMainPlayerController::OnMouseLeftClick()
{
    if (!IsCombatMode() || !GetTurnComp(GetCurPlayer())->IsMyTurn()) 
    {
        return;
    }

    FVector Start, End;
    TArray<AActor*> Ignores;
    FHitResult HitResult;
    //마우스 좌표를 월드 좌표로 해서 가져오기
    DeprojectMousePositionToWorld(Start, End);
    End = Start + (End * MaxTraceLength);

    bool bIsHitted = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1,
        false, Ignores, EDrawDebugTrace::None, HitResult, true);

    if (!bIsHitted) 
    {
        return;
    }

    WeaponComp = GetCurPlayer()->GetWeaponComponent();
    ACharacter* Target = Cast<ACharacter>(HitResult.GetActor());
    if (Target && WeaponComp)
    {
        if (Target->IsA<AEnemy>()) // 적 클릭
        {
            if (WeaponComp->IsMagicMode() && BasicUI->GetSelectedSkillName() == "Shield") 
            {
                return;
            }

            GetCurPlayer()->SetCombatTarget(Target);

            if (GetCurPlayer()->CanAttack())
            {
                GetCurPlayer()->Attack();
            }
            return;
        }
        else if (Target->IsA<ACPlayer>())
        {
            if (WeaponComp->IsMagicMode()) // 마법 모드일 때만
            {
                GetCurPlayer()->SetCombatTarget(Target);

                if (GetCurPlayer()->CanAttack())
                {
                    GetCurPlayer()->Attack();
                }
            }
        }
    }

    // 바닥 클릭
    if (GetCurPlayerStateComp()->IsPrepareMode()) // 공격 준비 상태였으면
    {
        CombatUI->ShowMessage(EMessage::NoTarget);
        return;
    }

    FNavLocation ProjectedLoc;
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    bool bIsValidPoint = NavSystem->ProjectPointToNavigation(HitResult.Location, ProjectedLoc);
    if (!bIsValidPoint) // AI가 갈 수 없는 곳
    {
        CombatUI->ShowMessage(EMessage::WrongSpot);
        return;
    }

    if (GetCurPlayerStateComp()->IsAttackMode()) 
    {
        return;
    }
   
    float DistandToTargetLoc = UKismetMathLibrary::Vector_Distance(GetCurPlayer()->GetActorLocation(), ProjectedLoc);

    bool bCanMove = GetTurnComp(GetCurPlayer())->CanMove(DistandToTargetLoc);

    if (bCanMove) // 클릭한 위치까지 (현재 이동력으로) 이동 가능한지
    {
        FTransform CircleTransform;
        CircleTransform.SetLocation(ProjectedLoc.Location);
        CircleTransform.SetScale3D(FVector(1.f, 0.4f, 0.4f));

        if (TargetSpotCircle) 
        {
            TargetSpotCircle->Destroy();
        }
        TargetSpotCircle = GetWorld()->SpawnActor<ATargetingCircle>(TargetingCircleClass, CircleTransform);
        if (TargetSpotCircle) 
        {
            TargetSpotCircle->SetColorGreen();
        }

        ClickedLocation = ProjectedLoc.Location;
        RemainingDist = DistandToTargetLoc;

        GetCurPlayer()->MoveInCombat(ClickedLocation);
        GetWorld()->GetTimerManager().SetTimer(CheckDistTimer, this, &AMainPlayerController::UpdateMovingAbility, 0.05f, true);
        PlayerLastLocation = GetCurPlayer()->GetActorLocation();
        GetWorld()->GetTimerManager().SetTimer(CheckMovingTimer, this, &AMainPlayerController::CheckPlayerMovingState, 1.f, true);
    }
    else // 이동 불가
    {
        MoveComp = CHelpers::GetComponent<UMyMovementComponent>(GetCurPlayer());
        if(MoveComp)
        {
            MoveComp->SetStandMode();
        }
        CombatUI->ShowMessage(EMessage::CannotMove);
    }
}

ACharacter* AMainPlayerController::FindNearestTarget(ACharacter* Self)
{
    AActor* NearestTarget = nullptr;
    TArray<AActor*> Targets;

    if (Self->IsA<ACPlayer>())    // 본인이 아군이라면 타겟은 적군 캐릭터들
    {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Targets);
    }
    else if (Self->IsA<AEnemy>()) // 본인이 적군이라면 타겟은 아군 캐릭터들
    {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPlayer::StaticClass(), Targets);
    }

    float NearestDistance = std::numeric_limits<float>::max(); // float 최대값으로 초기화

    for (AActor* Target : Targets)
    {
        float DistandToTarget = UKismetMathLibrary::Vector_Distance(Target->GetActorLocation(), Self->GetActorLocation());

        if (DistandToTarget < NearestDistance) // 본인과 가장 가까이 있는 타겟 찾기
        {
            NearestDistance = DistandToTarget;
            NearestTarget = Target;
        }
    }

    if (NearestTarget) 
    {
        return Cast<ACharacter>(NearestTarget);
    }

    return nullptr;
}

void AMainPlayerController::FinishCombat()
{
    bIsCombatMode = false;
    CombatUI->RemoveFromViewport();
    BasicUI->HideEndTurnButton();
    SetFixedCamera(false);

    for (ACharacter* CombatCharacter : CombatCharacters)
    {
        ACPlayer* PlayerCharacter = Cast<ACPlayer>(CombatCharacter);
        if (!PlayerCharacter)
        {
            return;
        }

        PlayerCharacter->SetCombatTarget(nullptr);

        StateComp = CHelpers::GetComponent<UStateComponent>(PlayerCharacter);
        if (StateComp)
        {
            StateComp->SetIdleMode();
        }

        TurnComp = GetTurnComp(PlayerCharacter);
        if (TurnComp)
        {
            TurnComp->SetTurn(false);
        }

        WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(PlayerCharacter);
        if (WeaponComp)
        {
            WeaponComp->UnEquip();
        }

        MoveComp = CHelpers::GetComponent<UMyMovementComponent>(PlayerCharacter);
        if (MoveComp)
        {
            MoveComp->SetWalkMode();
        }
        
        BasicUI->RemoveAllSkillHoverEffect();
    }

    CombatCharacterInfos.Empty();
    CombatCharacters.Empty();

    BasicUI->EnableSkillButtons();
}

void AMainPlayerController::StartFollowingPlayer()
{
    for (ACPlayer* PlayerCharacter : *BasicUI->GetPlayers())
    {
        if (PlayerCharacter != GetCurPlayer())
        {
            PlayerCharacter->FollowCurrentPlayer();
        }
    }
}

void AMainPlayerController::CancelSelectedSkill()
{
    BasicUI->CancelSelectedSkill();
}

void AMainPlayerController::UpdateBulletText(int32 CurBullet, int32 MaxBullet)
{
    BasicUI->UpdateBulletText(CurBullet, MaxBullet);
}

float AMainPlayerController::GetSkillDamage()
{
    return BasicUI->GetSkillDamage();
}

FString AMainPlayerController::GetSelectedSkillName()
{
    return BasicUI->GetSelectedSkillName();
}

void AMainPlayerController::EndTurn()
{
    TurnComp = GetTurnComp(GetCurPlayer());
    StateComp = GetCurPlayerStateComp();
    if (!TurnComp || !StateComp || !TurnComp->IsMyTurn() || StateComp->IsMovingMode() || StateComp->IsAttackMode())
    {
        return;
    }

    if (StateComp->IsPrepareMode() && BasicUI)
    {
        BasicUI->StopPrepareMode();
    }

    StateComp->SetWaitMode();
    StartTurn(TurnComp->GetTurnNum());
}
