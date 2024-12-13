// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/EnemyWeapon.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Character/Enemy.h"

AEnemyWeapon::AEnemyWeapon()
{
   Capsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
   Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AEnemyWeapon::BeginPlay()
{
    Super::BeginPlay();

    Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemyWeapon::OnComponentBeginOverlap);
}

void AEnemyWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor->IsA<ACPlayer>()) 
    {
        return;
    }
    Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AEnemyWeapon::ApplyDamageToTarget(ACharacter* OtherCharacter)
{
    ACPlayer* Target = Cast<ACPlayer>(OtherCharacter);
    if (Target)
    {
        FSetElementId ElementId = Hitted.Add(Target);
        if(ElementId.IsValidId())
        {
            AEnemy* enemy = Cast<AEnemy>(OwnerCharacter);
            float damage = UKismetMathLibrary::RandomFloatInRange(enemy->GetMinDamage(), enemy->GetMaxDamage());
            TSubclassOf<UDamageType> DamageType;
            UGameplayStatics::ApplyDamage(Target, damage, nullptr, this, DamageType);
        }
    }
}
