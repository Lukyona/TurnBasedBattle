// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
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

    CHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, TEXT("BehaviorTree'/Game/Enemies/AI/EnemyBT.EnemyBT'"));
    CHelpers::GetClass<AController>(&AIControllerClass, "Class'/Script/TB_Project.EnemyController'");

    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetRootComponent());
    Capsule->SetCollisionProfileName("Enemy", true);
   
    AgroSphere->SetRelativeLocation(FVector(350.f, 0.f, 0.f));
    AgroSphere->SetSphereRadius(700.f);
    FCollisionResponseContainer Cont;
    Cont.SetAllChannels(ECollisionResponse::ECR_Overlap);
    AgroSphere->SetCollisionResponseToChannels(Cont);
    AgroSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}
/*
void AEnemy::SetMeshAndAnim(const FString& MeshPath, const FString& AnimClassPath)
{
    //USkeletalMesh* Mesh;
    //CHelpers::GetAsset<USkeletalMesh>(&Mesh, *MeshPath);
    //GetMesh()->SetSkeletalMesh(Mesh);

    //TSubclassOf<UEnemyAnimInstance> AnimInstance;
    //CHelpers::GetClass<UEnemyAnimInstance>(&AnimInstance, *AnimClassPath);
    //GetMesh()->SetAnimClass(AnimInstance);
}
*/
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAgroSphereBeginOverlap);

    if (!WeaponClass) return;

    FTransform transform;
    FActorSpawnParameters params;
    params.Owner = this;
    Weapon = GetWorld()->SpawnActor<AEnemyWeapon>(WeaponClass, transform, params);
    Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);

}

void AEnemy::Tick(float DeltaTime)
{
    if (StateComponent->IsDeadMode()) return;
    
    Super::Tick(DeltaTime);
}


void AEnemy::NotifyActorBeginCursorOver()
{
    if(HealthComponent->IsAlive())
        MAINPC->SpawnTargetCharacterCircle(this);

    int cur = HealthComponent->GetHealth();
    int max = HealthComponent->GetMaxHealth();

    MAINPC->ShowEnemyInfo(GetName(), cur, max);
}

void AEnemy::NotifyActorEndCursorOver()
{
    if (HealthComponent->IsAlive())
        MAINPC->DestroyTargetCharacterCircle();

    MAINPC->HideEnemyInfo();
}

void AEnemy::Attack()
{
    if (bIsAttacking) return;

    bIsAttacking = true;
    int idx = UKismetMathLibrary::RandomIntegerInRange(0, AttackMontages.Num() - 1);
    PlayAnimMontage(AttackMontages[idx]);
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
        if (HitMontage) PlayAnimMontage(HitMontage);
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
    if (bIsCombatMode) return;
    if (!OtherActor->ActorHasTag("Player")) return;

    // 근처 적들(같은 태그) 모두 전투 모드로
    TArray<AActor*> characters;
    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AEnemy::StaticClass(), Tags[0], characters);

    for (AActor* actor : characters)
    {
        AEnemy* enemy = Cast<AEnemy>(actor);
        enemy->Behavior->SetCombatMode(true);
        MAINPC->AddCombatCharacter(Cast<AGameCharacter>(actor));
    }

    UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACPlayer::StaticClass(), "Player" , characters);
    for (AActor* player : characters)
    {
        MAINPC->AddCombatCharacter(Cast<AGameCharacter>(player));
    }

    MAINPC->StartCombat();
}

void AEnemy::Dead()
{
    AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (DeadMontage) PlayAnimMontage(DeadMontage);

    MAINPC->ExcludeCharacterInCombat(this);

    if (!DeadSound) return;

    USoundBase* deadSound = Cast<USoundBase>(DeadSound);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), deadSound, GetActorLocation());
}

void AEnemy::EndDead()
{
    GetMesh()->bPauseAnims = true;
}
