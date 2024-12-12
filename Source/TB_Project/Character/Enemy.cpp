// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Sound/SoundBase.h"

#include "Global.h"
#include "Components/MyMovementComponent.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/AIBehaviorComponent.h"
#include "EnemyAI/EnemyController.h"
#include "Character/EnemyAnimInstance.h"
#include "Character/CPlayer.h"
#include "Weapons/EnemyWeapon.h"

AEnemy::AEnemy()
{
    CHelpers::CreateComponent<USphereComponent>(this, &AgroSphere, "AgroSphere", GetRootComponent());
    AgroSphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    AgroSphere->SetCollisionProfileName("OverlapAll");

    SpringArm->SetRelativeLocation(FVector(0, 0, 50));
    SpringArm->SetRelativeRotation(FRotator(-30, 90, 0));
    SpringArm->TargetArmLength = 600.f;

    CHelpers::CreateActorComponent<UAIBehaviorComponent>(this, &Behavior, "BehaviorComponent");

    CHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, "BehaviorTree'/Game/Enemies/AI/EnemyBT.EnemyBT'");
    CHelpers::GetClass<AController>(&AIControllerClass, "Class'/Script/TB_Project.EnemyController'");

    CHelpers::GetAsset<UDataTable>(&EnemyDataTable, "DataTable'/Game/DataTables/EnemyData.EnemyData'");

    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetRootComponent());
    Capsule->SetCollisionProfileName("Enemy", true);
   
    AgroSphere->SetRelativeLocation(FVector(350.f, 0.f, 0.f));
    AgroSphere->SetSphereRadius(700.f);
    FCollisionResponseContainer Cont;
    Cont.SetAllChannels(ECollisionResponse::ECR_Overlap);
    AgroSphere->SetCollisionResponseToChannels(Cont);
    AgroSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

    Tags.Add("Enemy");
}

void AEnemy::InitializeFromDataTable(const FName& RowName)
{
    if (!EnemyDataTable)
    {
        return;
    }

    FEnemyData* Row = EnemyDataTable->FindRow<FEnemyData>(RowName, "");
    if (Row)
    {
        SetMeshAndAnim(Row->MeshPath, Row->AnimBlueprintPath);

        MinAttackDistance = Row->MinAttackDistance;
        MinDamage = Row->MinDamage;
        MaxDamage = Row->MaxDamage;

        MovementComponent->SetWalkSpeed(Row->WalkSpeed);
        MovementComponent->SetRunSpeed(Row->RunSpeed);

        TurnComponent->SetOriginMoveingAbility(Row->MovingAbility);
        TurnComponent->SetOriginActionAbility(Row->ActionAbility);

        HealthComponent->SetHealth(Row->MaxHealth);
        HealthComponent->SetMaxHealth(Row->MaxHealth);

        CHelpers::GetClass<AEnemyWeapon>(&WeaponClass, Row->WeaponClassPath);
        WeaponSocketName = Row->WeaponSocketName;

        AttackMontages = Row->AttackMontages;
        HitMontage = Row->HitMontage;
        DeadMontage = Row->DeadMontage;
    }
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAgroSphereBeginOverlap);

    if (!WeaponClass)
    {
        return;
    }

    FTransform Transform;
    FActorSpawnParameters Params;
    Params.Owner = this;
    Weapon = GetWorld()->SpawnActor<AEnemyWeapon>(WeaponClass, Transform, Params);
    if(Weapon)
    {
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
    }
}

void AEnemy::Tick(float DeltaTime)
{
    if (StateComponent->IsDeadMode()) 
    {
        return;
    }
    
    Super::Tick(DeltaTime);
}

void AEnemy::NotifyActorBeginCursorOver()
{
    if (!MAINPC || !HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("MAINPC or HealthComponent is null. NotifyActorBeginCursorOver skipped."));
        return;
    }

    if(HealthComponent->IsAlive())
    {
        MAINPC->SpawnTargetCharacterCircle(this);
    }

    int32 CurHealth = HealthComponent->GetHealth();
    int32 MaxHealth = HealthComponent->GetMaxHealth();

    MAINPC->ShowEnemyInfo(GetName(), CurHealth, MaxHealth);
}

void AEnemy::NotifyActorEndCursorOver()
{
    if (!MAINPC || !HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("MAINPC or HealthComponent is null. NotifyActorBeginCursorOver skipped."));
        return;
    }
    if (HealthComponent->IsAlive())
    {
        MAINPC->DestroyTargetCharacterCircle();
    }

    MAINPC->HideEnemyInfo();
}

void AEnemy::Attack()
{
    if (bIsAttacking) 
    {
        return;
    }

    bIsAttacking = true;
    int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, AttackMontages.Num() - 1);
    PlayAnimMontage(AttackMontages[Index]);
}

void AEnemy::EndAttack()
{
    bIsAttacking = false;
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (HealthComponent->IsAlive())
    {
        if (HitMontage) 
        {
            PlayAnimMontage(HitMontage);
        }
    }
    else
    {
        Dead();
        StateComponent->SetDeadMode();
    }

    MAINPC->UpdateEnemyHealth(this);

    return 0.0f;
}

void AEnemy::OnAgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bIsCombatMode || !OtherActor->ActorHasTag("Player") || !MAINPC)
    {
        return;
    }

    // 근처 적들(같은 태그) 모두 전투 모드로
    TArray<AActor*> Characters;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AEnemy::StaticClass(), Tags[0], Characters);
    for (AActor* Actor : Characters)
    {
        AEnemy* Enemy = Cast<AEnemy>(Actor);
        Enemy->Behavior->SetCombatMode(true);
        MAINPC->AddCombatCharacter(Cast<AGameCharacter>(Actor));
    }

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACPlayer::StaticClass(), "Player" , Characters);
    for (AActor* Player : Characters)
    {
        MAINPC->AddCombatCharacter(Cast<AGameCharacter>(Player));
    }

    MAINPC->StartCombat();
}

void AEnemy::Dead()
{
    AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (DeadMontage)
    {
        PlayAnimMontage(DeadMontage);
    }

    MAINPC->ExcludeCharacterInCombat(this);
}

void AEnemy::EndDead()
{
    GetMesh()->bPauseAnims = true;
}
