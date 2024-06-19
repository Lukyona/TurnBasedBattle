// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/CombatCharacterInfo.h"
#include "MainPlayerController.generated.h"

#define MAINPC AMainPlayerController::GetMainPC()

class UBasicUI;
class UCombatUI;
class ACPlayer;
class ACharacter;
class AEnemy;
class UDataTable;
class UAnimMontage;
class ATargetingCircle;
class UStateComponent;
class UTurnComponent;

UCLASS()
class TB_PROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMainPlayerController();

	static AMainPlayerController* MainPlayerController;

	bool bCombatMode;
	bool bFixedCamera;
	
	FVector ClickedLocation;
	FVector PlayerPrevLocation;
	float RemainingDist;

	FTimerHandle TH_CheckDist;
	FTimerHandle TH_CheckMoving;

	UBasicUI* BasicUI;
	UCombatUI* CombatUI;

	TArray<ACharacter*> CombatCharacters;
	TArray<FCombatCharacterInfo> CombatCharacterInfos;

	ATargetingCircle* TargetCharacterCircle;
	ATargetingCircle* TargetSpotCircle;

	UStateComponent* StateComp;
	UTurnComponent* TurnComp;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UBasicUI> BasicUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UCombatUI> CombatUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<ATargetingCircle> TargetingCircleClass;

	UStateComponent* GetCurPlayerStateComp();
	UTurnComponent* GetTurnComp(ACharacter* Char);

	virtual void BeginPlay() override;
public:
	static void SetMainPC(APlayerController* Controller);

    UFUNCTION(BlueprintCallable, BlueprintPure)
	static AMainPlayerController* GetMainPC() { return MainPlayerController; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCombatMode() { return bCombatMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getter")
		ACPlayer* GetCurPlayer();
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getter")
	UBasicUI* GetBasicUI() { return BasicUI; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getter")
	UCombatUI* GetCombatUI() { return CombatUI; }

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetFixedCamera(bool Value) { bFixedCamera = Value; }
	UFUNCTION(BlueprintCallable, Category = "Camera")
	bool IsFixedCamera() { return bFixedCamera; }

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SpawnTargetCharacterCircle(ACharacter* Target);
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void DestroyTargetCharacterCircle();
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void ShowEnemyInfo(FString Name, int CurHealth, int MaxHealth);
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void HideEnemyInfo();
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void UpdateEnemyHealth(AEnemy* Enemy);

	UFUNCTION(BlueprintCallable, Category = "Skill")
		void SetSkills(UDataTable* SkillDT);
	UFUNCTION(BlueprintCallable, Category = "Skill")
		void DisableSkillButtons();

	UFUNCTION(BlueprintCallable, Category = "PlayerAttack")
	void PlayAction(UAnimMontage* Anim, bool NeedTarget);
	UFUNCTION(BlueprintCallable, Category = "PlayerAttack")
	void PlayAttackPose(UAnimMontage* Anim);
	UFUNCTION(BlueprintCallable, Category = "PlayerAttack")
	void StopAttackPose();

	UFUNCTION(Category = "PlayerMove")
	void CheckRemainingDist();
	UFUNCTION(Category = "PlayerMove")
	void CheckPlayerNotMoving();
	UFUNCTION(Category = "PlayerMove")
	void ClearMoveAbilityTimer();

	UFUNCTION(BlueprintCallable, Category = "CombatSystem")
	void StartCombat();
	UFUNCTION(BlueprintCallable, Category = "CombatSystem")
	int GetRandomNumber(int Max);
	UFUNCTION(BlueprintCallable, Category = "CombatSystem")
	void StartTurn(int InCurNum);

	UFUNCTION(BlueprintCallable, Category = "MouseEvent")
	void ActivateMouseEvent();
	UFUNCTION(BlueprintCallable, Category = "MouseEvent")
	void DeactivateMouseEvent();
	UFUNCTION(BlueprintCallable, Category = "MouseEvent")
		void MouseLeftClick();

	UFUNCTION(BlueprintCallable, Category = "CombatCharacters")
	void AddCombatCharacter(AGameCharacter* NewCharacter);
	UFUNCTION(BlueprintCallable, Category = "CombatCharacters")
	void ExcludeCharacterInCombat(ACharacter* Char);
	UFUNCTION(BlueprintCallable, Category = "CombatCharacters")
	void UpdateCombatCharacters(ACharacter* Char);

	UFUNCTION(BlueprintCallable)
	void ChangePlayer(ACPlayer* NewPlayer);

	ACharacter* FindNearestTarget(ACharacter* Self);

	void FinishCombat();

	void StartFollowingPlayer();

	// BasicUI
	void CancelSelectedSkill();
	void UpdateBulletText(int CurBullet, int MaxBullet);
	float GetSkillDamage();
	FString GetSelectedSkillName();
};
