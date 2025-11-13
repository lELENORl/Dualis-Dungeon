// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyProjectCharacter.h"
#include "AIC_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AAIC_Enemy : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_Enemy(const class FObjectInitializer& ObjectInitializer);

public:
	void SetPlayerKey(APawn* player);

	UFUNCTION()
	AMyProjectCharacter* GetPlayerKey();

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName PlayerKeyName;

protected:
	// AIControllerのPawn所持
	virtual void OnPossess(class APawn* InPawn) override;

	// AIControllerのPawn所持解除
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	class UBehaviorTree* BehaviorTree;

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerception;//AIの感覚コンポーネント
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Sight* SightConfig;    //視覚で認識条件やパラメータを設定用
};
