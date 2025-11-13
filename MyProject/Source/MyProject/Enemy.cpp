// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "MyProjectCharacter.h"
#include "AIC_Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 視野
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	// 見える範囲
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePlayer);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::OnSeePlayer(APawn* Pawn)
{
	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(GetController());
	// プレイヤー
	AMyProjectCharacter* player = Cast<AMyProjectCharacter>(Pawn);

	if (AIController && player)
	{
		// AIControllerにプレイヤー情報を設定
		AIController->SetPlayerKey(player);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *player->GetActorLocation().ToString());
	}

	// 視野に入ったら画面に"See"と表示
	UKismetSystemLibrary::PrintString(this, "See", true, true, FColor::Blue, 2.f);
}
