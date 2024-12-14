// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/BasicUI.h"
#include "Components/Button.h"

#include "Global.h"
#include "Character/CPlayer_Revenant.h"
#include "Character/CPlayer_Aurora.h"
#include "Character/CPlayer_Sparrow.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Weapons/PlayerWeapon.h"
#include "Weapons/Gun.h"
#include "Widgets/CombatUI.h"
#include "Widgets/SkillButton.h"
#include "Widgets/SkillInfoBox.h"

void UBasicUI::NativeConstruct()
{
    Player1 = CHelpers::FindActor<ACPlayer_Revenant>(GetWorld());
    Player2 = CHelpers::FindActor<ACPlayer_Aurora>(GetWorld());
    Player3 = CHelpers::FindActor<ACPlayer_Sparrow>(GetWorld());

    CurPlayer = Player1;

    Players.Add(Player1);
    Players.Add(Player2);
    Players.Add(Player3);
}

void UBasicUI::ChangePlayer(ACPlayer* TargetPlayer)
{
    StateComp = CurPlayer->GetStateComponent();
    if (StateComp->IsMovingMode() || !MAINPC)
    {
        return;
    }

    if (StateComp->IsPrepareMode()) 
    {
        StopPrepareMode();
    }

    if (TargetPlayer->IsA<ACPlayer_Revenant>())
    {
        PlayerType = EPlayerType::Revenent;
    }
    if (TargetPlayer->IsA<ACPlayer_Aurora>()) 
    {
        PlayerType = EPlayerType::Aurora;
    }
    if (TargetPlayer->IsA<ACPlayer_Sparrow>()) 
    {
        PlayerType = EPlayerType::Sparrow;
    }
        
    CurPlayer = TargetPlayer;
    CurPlayer->ClearFollowTimer();
    CurPlayer->ResetSpringArm();
    MAINPC->Possess(CurPlayer);

    if (CurPlayer == Player1) 
    {
        ShowMagicButton();
    }
    else 
    {
        HideMagicButton();
    }

    WeaponComp = CurPlayer->GetWeaponComponent();
    if(WeaponComp) 
    {
        WeaponComp->SetSkillIcons();
    }

    if (MAINPC->IsCombatMode()) 
    {
        CheckTurnState();
    }

    for (ACPlayer* Player : Players)
    {
        if (Player == CurPlayer) 
        {
            continue;
        }

        Player->PossessAIController();

        if (!MAINPC->IsCombatMode())
        {
            Player->FollowCurrentPlayer();
        }
    }

    SetPlayerImg();
}

void UBasicUI::EnableSkillButtons()
{
    WeaponComp = CurPlayer->GetWeaponComponent();

    for (USkillButton* SkillButton : SkillButtons)
    {
        SkillButton->SetIsEnabled(true);

        AGun* Gun = Cast<AGun>(WeaponComp->GetEquippedWeapon());
        if (Gun && Gun->GetCurBulletAmount() <= 2)
        {
            DisableSkillsBasedOnBullets(Gun->GetCurBulletAmount());
        }
    }
}

void UBasicUI::DisableSkillButtons()
{
    for (USkillButton* SkillButton : SkillButtons)
    {
        SkillButton->SetIsEnabled(false);
    }
}

void UBasicUI::UpdateSkillButton(FString NewSelectedSkillName)
{
    if (NewSelectedSkillName == SelectedSkillName) 
    {
        return;
    }

    for (USkillButton* SkillButton : SkillButtons)
    {
        if (SkillButton->GetSkillName() == SelectedSkillName)
        {
            SkillButton->CancelSelection();
        }
    }

    SelectedSkillName = NewSelectedSkillName;
}

void UBasicUI::CancelSelectedSkill()
{
    for (USkillButton* SkillButton : SkillButtons)
    {
        if (SkillButton->IsSelectedNow())
        {
            SkillButton->CancelSelection();
        }
    }

    StateComp = CurPlayer->GetStateComponent();
    StateComp->SetWaitMode();
}

void UBasicUI::CheckTurnState()
{
    TurnComp = CurPlayer->GetTurnComponent();
    if (!TurnComp || !MAINPC)
    {
        return;
    }

    if (TurnComp->IsMyTurn())
    {
        MAINPC->GetCombatUI()->HideMessage();
        EnableSkillButtons();
        EnableEndTurnButton();
    }
    else
    {
        MAINPC->GetCombatUI()->ShowMessage(EMessage::NotTurn);
        DisableSkillButtons();
        DisableEndTurnButton();
    }
}

void UBasicUI::HideSkillInfo()
{
    if (SkillInfoBox)
    {
        SkillInfoBox->SetVisibility(ESlateVisibility::Hidden);
    }
}

int32 UBasicUI::GetSkillDamage()
{
    for (USkillButton* SkillButton : SkillButtons)
    {
        if (SkillButton->IsSelectedNow())
        {
            return SkillButton->GetRandomDamage();
        }
    }

    return 0;
}

FString UBasicUI::SetHealthText(ACharacter* Character)
{
    UHealthComponent* HealthComp = CHelpers::GetComponent<UHealthComponent>(Character);
    if (!HealthComp)
    {
        return "";
    }
    FString CurHealth = FString::FromInt(HealthComp->GetHealth());
    FString MaxHealth = FString::FromInt(HealthComp->GetMaxHealth());

    return CurHealth + "/" + MaxHealth;
}

float UBasicUI::SetHealthProgressBar(ACharacter* Character)
{
    UHealthComponent* HealthComp = CHelpers::GetComponent<UHealthComponent>(Character);
    if (!HealthComp)
    {
        return -1.f;
    }
    float CurHealth = HealthComp->GetHealth();
    float MaxHealth = HealthComp->GetMaxHealth();

    return 1.f - (CurHealth / MaxHealth);
}

float UBasicUI::GetActionAbilityBarPercent()
{
    TurnComp = CurPlayer->GetTurnComponent();
    if (!TurnComp)
    {
        return - 1.f;
    }
    float CurAbility = TurnComp->GetCurActionAbility();
    float OriginAbility = TurnComp->GetOriginActionAbility();

    return (CurAbility / OriginAbility);
}

float UBasicUI::GetMovingAbilityBarPercent()
{
    TurnComp = CurPlayer->GetTurnComponent();
    if (!TurnComp)
    {
        return -1.f;
    }
    float CurAbility = TurnComp->GetCurMovingAbility();
    float OriginAbility = TurnComp->GetOriginMovingAbility();

    return (CurAbility / OriginAbility) - 0.09f;
}

void UBasicUI::DisableSkillsBasedOnBullets(int32 CurBullet)
{
    if (CurBullet > 2) 
    {
        return;
    }

    int32 Min = 0;
    if (CurBullet >= 1) 
    {
        Min = CurBullet + 1;
    }

    //CurBullet == 0 → 범위 0~3 (스킬 0, 1, 2, 3 모두 비활성화)
    //CurBullet == 1 → 범위 2~3 (스킬 2, 3 비활성화)
    //CurBullet == 2 → 범위 3~3 (스킬 3 비활성화)
    for (int i = 0; i < SkillButtons.Num(); ++i)
    {
        // 이 범위 안에 들어온다면 해당 스킬은 총알 부족으로 사용 불가
        bool bIsValid = UKismetMathLibrary::InRange_IntInt(i, Min, 3); 
        SkillButtons[i]->SetIsEnabled(!bIsValid);
    }
}

void UBasicUI::ToggleMagicMode()
{
    if (CurPlayer != Player1) //플레이어1(레버넌트)만 마법 사용 가능
    {
        return;
    }

    WeaponComp = CurPlayer->GetWeaponComponent();
    if (WeaponComp)
    {
        WeaponComp->ToggleMagicMode();
    }
}

void UBasicUI::StopPrepareMode()
{
    if (MAINPC)
    {
        MAINPC->StopAttackPose();
    }

    for (USkillButton* SkillButton : SkillButtons)
    {
        if (SkillButton->IsSelectedNow())
        {
            SkillButton->RemoveSkillHoverEffect();
            break;
        }
    }
}

void UBasicUI::RemoveAllSkillHoverEffect()
{
    for (USkillButton* SkillButton : SkillButtons)
    {
        SkillButton->RemoveSkillHoverEffect();
    }
}
