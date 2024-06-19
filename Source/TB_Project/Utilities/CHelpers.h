// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "kismet/GameplayStatics.h"		
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

class TB_PROJECT_API CHelpers
{
public:
    template<typename T>
    static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
    {
        *OutComponent = InActor->CreateDefaultSubobject<T>(InName);
        if (InParent)
        {
			// SetupAttachment()�� �����ڿ����� ����
            (*OutComponent)->SetupAttachment(InParent,InSocketName);
            return;
        }
        InActor->SetRootComponent(*OutComponent);
    }

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);		
	}

    template<typename T>
    static void GetAsset(T** OutObject, FString InPath) // �������Ʈ Ŭ���� ��������
    {
        ConstructorHelpers::FObjectFinder<T> asset(*InPath);
        *OutObject = asset.Object;
    }

    template<typename T>
    static void GetAssetDynamic(T** OutObject, FString InPath)
    {
        *OutObject = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
    }

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutObject, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutObject = asset.Class;
	}

	template<typename T>
	static T* FindActor(UWorld* InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (actor && actor->IsA<T>()) // ���Ͱ� TŸ���� �´ٸ�
			{
				return Cast<T>(actor);
			}
		}
		return nullptr;
	}

	template<typename T>
	static void FindActors(UWorld* InWorld, TArray<T*>& OutActors)
	{
		OutActors.Empty();
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (actor  && actor->IsA<T>())
				OutActors.Add(Cast<T>(actor));
		}
	}

	static void AttachTo(AActor* InActor, USceneComponent* InParent, const FName& InSocketName)
	{
		InActor->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
	}

    template<typename T>
    static T*  GetComponent(AActor* InActor)
    {
        return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
    }

    /*
    static void PlayEffect(UWorld* InWorld, UFXSystemAsset* InAsset, const FTransform& InTransform,
        USkeletalMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None)
    {
        UParticleSystem* particle = Cast<UParticleSystem>(InAsset);
        UNiagaraSystem*  niagara  = Cast<UNiagaraSystem>(InAsset);

		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

        if (!!InMesh)
        {
            if (!!particle)
            {
                UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, InTransform);
            }
            if (!!niagara)
            {
				UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, InMesh, InSocketName, location, rotation, scale, EAttachLocation::KeepRelativeOffset, true,
					ENCPoolMethod::None);
            }


            return;
        }
        if (!!particle)
        {
            UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, InTransform);
            return;
        }
        if (!!niagara)
        {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara,location,rotation, scale);
            return;
        }

    }
    static ACGhostTrail* Play_GhostTrail(TSubclassOf<ACGhostTrail>& InClass, ACharacter* InOwner)
    {
        CheckNullResult(InClass, nullptr);
        CheckNullResult(InOwner, nullptr);

        FTransform transform;
        FVector vector = InOwner->GetActorLocation();
        vector.Z -= InOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        transform.SetTranslation(vector);


        FActorSpawnParameters params;
        params.Owner = InOwner;
        params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        

        return InOwner->GetWorld()->SpawnActor<ACGhostTrail>(InClass, transform, params);

    }*/



};
