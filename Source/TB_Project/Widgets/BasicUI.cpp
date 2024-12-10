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

void UBasicUI::EndTurn()
{
    TurnComp = CurPlayer->GetTurnComponent();
    StateComp = CurPlayer->GetStateComponent();
    if (!TurnComp->IsMyTurn() || StateComp->IsMovingMode() || StateComp->IsAttackMode())
        return;

    if (StateComp->IsPrepareMode())
        StopPrepareMode();

    StateComp->SetWaitMode();
    MAINPC->StartTurn(TurnComp->GetTurnNum());
}

void UBasicUI::ChangePlayer(ACPlayer* TargetPlayer)
{
    StateComp = CurPlayer->GetStateComponent();
    if (StateComp->IsMovingMode()) return;

    if (StateComp->IsPrepareMode()) StopPrepareMode();

    if (TargetPlayer->IsA<ACPlayer_Revenant>()) PlayerType = EPlayerType::Revenent;
    if (TargetPlayer->IsA<ACPlayer_Aurora>()) PlayerType = EPlayerType::Aurora;
    if (TargetPlayer->IsA<ACPlayer_Sparrow>()) PlayerType = EPlayerType::Sparrow;
        
    CurPlayer = TargetPlayer;

    CurPlayer->ClearFollowTimer();
    CurPlayer->ResetSpringArm();
    MAINPC->Possess(CurPlayer);

    if (CurPlayer == Player1) ShowMagicButton();
    else HideMagicButton();

    WeaponComp = CurPlayer->GetWeaponComponent();
    if(WeaponComp) WeaponComp->SetSkillIcons();

    if (MAINPC->IsCombatMode()) CheckTurnState();

    for (ACPlayer* player : Players)
    {
        if (player == CurPlayer) continue;

        player->PossessAIController();

        if (!MAINPC->IsCombatMode())
            player->FollowingCurrentPlayer();
    }

    SetPlayerImg();
}

void UBasicUI::EnableSkillButtons()
{
    WeaponComp = CurPlayer->GetWeaponComponent();

    for (USkillButton* sb : SkillButtons)
    {
        sb->SetIsEnabled(true);
        AGun* gun = Cast<AGun>(WeaponComp->GetEquippedWeapon());
        if (gun)
        {
            if (gun->GetCurBulletAmount() <= 2)
                UpdateGunSkills(gun->GetCurBulletAmount());
        }
    }

    
}

void UBasicUI::DisableSkillButtons()
{
    for (USkillButton* sb : SkillButtons)
    {
        sb->SetIsEnabled(false);
    }
}

void UBasicUI::UpdateSkillButton(FString NewSelectedSkillName)
{
    if (NewSelectedSkillName == SelectedSkillName) return;

    for (USkillButton* sb : SkillButtons)
    {
        if (sb->GetSkillName() == SelectedSkillName)
            sb->CancelSelection();
    }

    SelectedSkillName = NewSelectedSkillName;
}

void UBasicUI::CancelSelectedSkill()
{
    for (USkillButton* sb : SkillButtons)
    {
        if (sb->IsSelectedNow())
        {
            sb->CancelSelection();
        }
    }

    StateComp = CurPlayer->GetStateComponent();
    StateComp->SetWaitMode();
}

void UBasicUI::CheckTurnState()
{
    TurnComp = CurPlayer->GetTurnComponent();
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
        SkillInfoBox->SetVisibility(ESlateVisibility::Hidden);
}

int UBasicUI::GetSkillDamage()
{
    for (USkillButton* sb : SkillButtons)
    {
        if (sb->IsSelectedNow())
            return sb->GetRandomDamage();
    }

    return 0;
}

FString UBasicUI::SetHealthText(ACharacter* Character)
{
    UHealthComponent* healthComp = CHelpers::GetComponent<UHealthComponent>(Character);
    FString CurHealth = FString::FromInt(healthComp->GetHealth());
    FString MaxHealth = FString::FromInt(healthComp->GetMaxHealth());

    return CurHealth + "/" + MaxHealth;
}

float UBasicUI::SetHealthProgressBar(ACharacter* Character)
{
    UHealthComponent* healthComp = CHelpers::GetComponent<UHealthComponent>(Character);
    float CurHealth = healthComp->GetHealth();
    float MaxHealth = healthComp->GetMaxHealth();

    return 1.f - (CurHealth / MaxHealth);
}

float UBasicUI::GetActionAbilityBarPercent()
{
    TurnComp = CurPlayer->GetTurnComponent();
    float CurAbility = TurnComp->GetCurActionAbility();
    float OriginAbility = TurnComp->GetOriginActionAbility();

    return (CurAbility / OriginAbility);
}

float UBasicUI::GetMovingAbilityBarPercent()
{
    TurnComp = CurPlayer->GetTurnComponent();
    float CurAbility = TurnComp->GetCurMovingAbility();
    float OriginAbility = TurnComp->GetOriginMovingAbility();

    return (CurAbility / OriginAbility) - 0.09f;
}

void UBasicUI::UpdateGunSkills(int CurBullet)
{
    if (CurBullet > 2) return;

    int min = 0;
    if (CurBullet == 0) min = 0;
    if (CurBullet == 1) min = 2;
    if (CurBullet == 2) min = 3;

    for (int i = 0; i < SkillButtons.Num(); ++i)
    {
        // 이 범위 안에 들어온다면 해당 스킬은 총알 부족으로 사용 불가
        bool b = UKismetMathLibrary::InRange_IntInt(i, min, 3); 
        SkillButtons[i]->SetIsEnabled(!b);
    }
}

void UBasicUI::ToggleMagicMode()
{
    if (CurPlayer != Player1) return;

    WeaponComp = CurPlayer->GetWeaponComponent();

    if (WeaponComp->GetCurrentWeaponType() == EWeaponType::Magic)
    {
        if (MAINPC->IsCombatMode())
        {
            WeaponComp->SetCurrentWeapon(EWeaponType::Gun);
            WeaponComp->SetMode(EWeaponType::Gun);
        }
        else
        {
            WeaponComp->SetMode(EWeaponType::UnArmed);
            WeaponComp->SetCurrentWeapon(EWeaponType::Gun);
        }
    }
    else
    {
        WeaponComp->SetCurrentWeapon(EWeaponType::Magic);
        WeaponComp->SetMode(EWeaponType::Magic);
    }
}

void UBasicUI::StopPrepareMode()
{
    MAINPC->StopAttackPose();

    for (USkillButton* sb : SkillButtons)
    {
        if (sb->IsSelectedNow())
        {
            sb->RemoveSkillHoverEffect();
            break;
        }
    }
}

void UBasicUI::RemoveAllSkillHoverEffect()
{
    for (USkillButton* sb : SkillButtons)
        sb->RemoveSkillHoverEffect();
}
