// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob_DamageNotify.h"
#include "BaseWeapon.h"

//void UMob_DamageNotify::NotifyBegin(
//    USkeletalMeshComponent* MeshComp,
//    UAnimSequenceBase* Animation,
//    float TotalDuration,
//    const FAnimNotifyEventReference& EventReference
//)
//{
//    // 実装
//}

bool UMob_DamageNotify::Received_NotifyBegin(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    float TotalDuration, const
    FAnimNotifyEventReference& EventReference) const
{
    if (!MeshComp)
        return false;

    // Owner Actor取得
    AActor* OwnerActor = MeshComp->GetOwner();
    if (!OwnerActor)
        return false;

    // ChildActorComponent から ChildActor を取得（もし存在する場合）
    UChildActorComponent* ChildActorComp = OwnerActor->FindComponentByClass<UChildActorComponent>();
    if (!ChildActorComp)
        return false;
    AActor* ChildActor = ChildActorComp->GetChildActor();
    if (!ChildActor)
        return false;

    ABaseWeapon* Weapon = Cast<ABaseWeapon>(ChildActor);
    if (!Weapon)
        return false;

    // 武器のメソッド呼び出し
    Weapon->SwordActive();

    return true;
}