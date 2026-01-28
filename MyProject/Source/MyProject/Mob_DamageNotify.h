// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Mob_DamageNotify.generated.h"

UCLASS()
class MYPROJECT_API UMob_DamageNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> WeaponClass;
protected:

    //virtual void NotifyBegin(
    //    USkeletalMeshComponent* MeshComp,
    //    UAnimSequenceBase* Animation,
    //    float TotalDuration,
    //    const FAnimNotifyEventReference& EventReference
    //) override;

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration
	) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation
	) override;
};
