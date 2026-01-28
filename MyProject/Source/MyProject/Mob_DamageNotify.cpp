// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob_DamageNotify.h"
#include "BaseWeapon.h"
#include "Enemy.h"
#include "Components/ChildActorComponent.h"

//void UMob_DamageNotify::NotifyBegin(
//    USkeletalMeshComponent* MeshComp,
//    UAnimSequenceBase* Animation,
//    float TotalDuration,
//    const FAnimNotifyEventReference& EventReference
//)
//{
//    // é¿ëï
//}

void UMob_DamageNotify::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration
)
{
	UE_LOG(LogTemp, Warning, TEXT("DamageNotify BEGIN"));

	if (!MeshComp) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;

	UChildActorComponent* ChildActorComp =
		OwnerActor->FindComponentByClass<UChildActorComponent>();
	if (!ChildActorComp) return;

	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(ChildActorComp->GetChildActor()))
	{
		Weapon->SwordActive();
	}
}

void UMob_DamageNotify::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation
)
{
	UE_LOG(LogTemp, Warning, TEXT("DamageNotify END"));

	if (!MeshComp) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;

	// ïêäÌOFF
	if (UChildActorComponent* ChildActorComp =
		OwnerActor->FindComponentByClass<UChildActorComponent>())
	{
		if (ABaseWeapon* Weapon =
			Cast<ABaseWeapon>(ChildActorComp->GetChildActor()))
		{
			Weapon->SwordDeactive();
		}
	}

	// çUåÇèIóπí ím
	if (AEnemy* Enemy = Cast<AEnemy>(OwnerActor))
	{
		Enemy->OnAttackFinished();
	}
}