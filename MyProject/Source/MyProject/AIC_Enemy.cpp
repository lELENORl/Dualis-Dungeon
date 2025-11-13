// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Enemy.h"
#include "UObject/ConstructorHelpers.h"

AAIC_Enemy::AAIC_Enemy(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// 作成したビヘイビアツリーを設定
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/TopDown/Blueprints/AI/BehaviorTree01.BehaviorTree01"));
	BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
}

void AAIC_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAIC_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AIControllerがPawn所持した際にBBとBTを使用
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorComp->StartTree(*BehaviorTree);
}

void AAIC_Enemy::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

void AAIC_Enemy::SetPlayerKey(APawn* player)
{
	ensure(BlackboardComp);

	// ブラックボードで作成したPlayerというキーにプレイヤー情報を入れる
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
	FVector playerPos = player->GetActorLocation();
	BlackboardComp->SetValueAsVector(FName("moveTO"), playerPos);
}

AMyProjectCharacter* AAIC_Enemy::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<AMyProjectCharacter>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}