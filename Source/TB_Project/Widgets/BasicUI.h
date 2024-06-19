// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicUI.generated.h"

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	Revenent, Aurora, Sparrow,
};

UCLASS()
class TB_PROJECT_API UBasicUI : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

protected:
    UPROPERTY(BlueprintReadOnly)
	EPlayerType PlayerType = EPlayerType::Revenent;

	UPROPERTY(BlueprintReadOnly)
	class ACPlayer* Player1;
	UPROPERTY(BlueprintReadOnly)
	class ACPlayer* Player2;
	UPROPERTY(BlueprintReadOnly)
	class ACPlayer* Player3;

	UPROPERTY(BlueprintReadOnly)
	class ACPlayer* CurPlayer;

	TArray<ACPlayer*> Players;

	class UTurnComponent* TurnComp;
	class UStateComponent* StateComp;
	class UPlayerWeaponComponent* WeaponComp;

    UPROPERTY(BlueprintReadWrite)
	TArray<class USkillButton*> SkillButtons;

	UPROPERTY(BlueprintReadWrite)
	FString SelectedSkillName;

	UPROPERTY(BlueprintReadWrite)
	class USkillInfoBox* SkillInfoBox;

	UFUNCTION(BlueprintCallable)
	void EndTurn();

public:
	UFUNCTION(BlueprintCallable)
	void ChangePlayer(ACPlayer* TargetPlayer);

	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerImg();

    UFUNCTION(BlueprintImplementableEvent)
		void ShowSkills(class UDataTable* SkillInfoDT);

	UFUNCTION(BlueprintCallable)
	void EnableSkillButtons();
	void DisableSkillButtons();

	UFUNCTION(BlueprintCallable)
	void UpdateSkillButton(FString NewSelectedSkillName);

	void CancelSelectedSkill();
	void CheckTurnState();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowSkillInfo();
	UFUNCTION(BlueprintCallable)
	void HideSkillInfo();

	int GetSkillDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowEndTurnButton();
	UFUNCTION(BlueprintImplementableEvent)
	void HideEndTurnButton();

	UFUNCTION(BlueprintImplementableEvent)
	void EnableEndTurnButton();
	UFUNCTION(BlueprintImplementableEvent)
	void DisableEndTurnButton();

	UFUNCTION(BlueprintCallable)
	FString SetHealthText(ACharacter* Character);
	UFUNCTION(BlueprintCallable)
	float SetHealthProgressBar(ACharacter* Character);

	UFUNCTION(BlueprintCallable)
	float GetActionAbilityBarPercent();
	UFUNCTION(BlueprintCallable)
	float GetMovingAbilityBarPercent();

	UFUNCTION(BlueprintImplementableEvent)
	void SetMovingAbilityBarPercent();
	UFUNCTION(BlueprintImplementableEvent)
	void ResetMovingAbilityBarPercent();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowBulletUIBox();
	UFUNCTION(BlueprintImplementableEvent)
	void HideBulletUIBox();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBulletText(int Cur, int Max);

	void UpdateGunSkills(int CurBullet);

	UFUNCTION(BlueprintImplementableEvent)
		void ShowMagicButton();
	UFUNCTION(BlueprintImplementableEvent)
		void HideMagicButton();

    UFUNCTION(BlueprintCallable)
	void ToggleMagicMode();

	void StopPrepareMode();

	void RemoveAllSkillHoverEffect();

	TArray<ACPlayer*>* GetPlayers() { return &Players; }

	FString GetSelectedSkillName() { return SelectedSkillName; }

};
