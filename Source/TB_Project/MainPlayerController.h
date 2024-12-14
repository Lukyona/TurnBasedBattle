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
protected:
	virtual void BeginPlay() override;


	static AMainPlayerController* MainPlayerController;

	bool bIsCombatMode;
	bool bIsFixedCamera;
	
	UPROPERTY()
	UStateComponent* StateComp;
	UPROPERTY()
	UTurnComponent* TurnComp;

	UPROPERTY()
	TArray<ACharacter*> CombatCharacters;
	TArray<FCombatCharacterInfo> CombatCharacterInfos;

	// �̵� ����
	FVector ClickedLocation, PlayerLastLocation;
	float RemainingDist;
	FTimerHandle CheckDistTimer, CheckMovingTimer;

	//UI Classes
	TSubclassOf<UBasicUI> BasicUIClass;
	TSubclassOf<UCombatUI> CombatUIClass;
	TSubclassOf<ATargetingCircle> TargetingCircleClass;

	UPROPERTY()
	UBasicUI* BasicUI;
	UPROPERTY()
	UCombatUI* CombatUI;

	UPROPERTY()
	ATargetingCircle* TargetCharacterCircle;
	UPROPERTY()
	ATargetingCircle* TargetSpotCircle;

public://Getters and setters
    UFUNCTION(BlueprintCallable, BlueprintPure)
	static AMainPlayerController* GetMainPC() 
	{
		return MainPlayerController; 
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCombatMode() 
	{
		return bIsCombatMode; 
	}

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getter")
	UBasicUI* GetBasicUI() 
	{
		return BasicUI; 
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getter")
	UCombatUI* GetCombatUI() 
	{
		return CombatUI; 
	}

	bool IsFixedCamera() 
	{
		return bIsFixedCamera; 
	}
	void SetFixedCamera(bool Value) 
	{
		bIsFixedCamera = Value; 
	}

	static void SetMainPC(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getter")
	ACPlayer* GetCurPlayer();

	UStateComponent* GetCurPlayerStateComp();
	UTurnComponent* GetTurnComp(ACharacter* InCharacter);


	//Core methods
	// �� UI ����
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SpawnTargetCharacterCircle(ACharacter* Target);
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void DestroyTargetCharacterCircle();
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void ShowEnemyInfo(FString Name, int32 CurHealth, int32 MaxHealth);
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void HideEnemyInfo();
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void UpdateEnemyHealth(AEnemy* Enemy);

	// ��ų UI ����
	void ShowSkills(UDataTable* SkillDT);
	void DisableSkillButtons();

	// �ִϸ��̼� ����
	UFUNCTION(BlueprintCallable, Category = "PlayerAttack")
	void PlayAction(UAnimMontage* Anim, bool bIsNeedTarget);
	UFUNCTION(BlueprintCallable, Category = "PlayerAttack")
	void PlayAttackPose(UAnimMontage* Anim);
	UFUNCTION(BlueprintCallable, Category = "PlayerAttack")
	void StopAttackPose();

	// ���� �� �÷��̾� �̵� ����
	void UpdateMovingAbility();
	void CheckPlayerMovingState();
	void ClearMoveAbilityTimer();

	//���콺 Ŭ�� ����
	void OnMouseLeftClick();
	UFUNCTION(BlueprintCallable, Category = "MouseEvent")
	void EnableMouseClickEvent();
	UFUNCTION(BlueprintCallable, Category = "MouseEvent")
	void DisableMouseClickEvent();

	// ���� ����
	void StartCombat();
	void FinishCombat();

	void StartTurn(int32 InCurNum);
	UFUNCTION(BlueprintCallable)
	void EndTurn();

	int32 GetRandomNumber(int32 Max);
	ACharacter* FindNearestTarget(ACharacter* Self);

	void AddCombatCharacter(AGameCharacter* NewCharacter);
	void ExcludeCharacterInCombat(ACharacter* DeadCharacter);
	void UpdateCombatCharacters(ACharacter* DeadCharacter);

	//�÷��̾� ���� ����
	void StartFollowingPlayer();
	UFUNCTION(BlueprintCallable)
	void ChangePlayer(ACPlayer* NewPlayer);


	// BasicUI�� �Լ��� ȣ������
	void CancelSelectedSkill();
	void UpdateBulletText(int32 CurBullet, int32 MaxBullet);
	float GetSkillDamage();
	FString GetSelectedSkillName();

};
