// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "MyProjectCharacter.h"
#include "AIC_Enemy.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

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

    /* ===========
      Weapon ChildActor
      =========== */
    //WeaponChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChild"));
    //WeaponChild->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

    //static ConstructorHelpers::FClassFinder<AActor> WeaponBP(
    //    TEXT("/Game/TopDown/Blueprints/BaseWeapon_MobFist")
    //);
    //if (WeaponBP.Succeeded())
    //{
    //    WeaponClass = WeaponBP.Class;
    //    WeaponChild->SetChildActorClass(WeaponClass);
    //}

    /* ===========
       Attack Animation
       =========== */
    static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageAsset(
        TEXT("/Game/TopDown/anim/mob/Mob_Attack_Montage.Mob_Attack_Montage")
    );
    if (AttackMontageAsset.Succeeded())
    {
        AttackMontage = AttackMontageAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageAsset(
        TEXT("/Game/TopDown/anim/mob/Mob_Death_Montage.Mob_Death_Montage")
    );
    if (DeathMontageAsset.Succeeded())
    {
        DeathMontage = DeathMontageAsset.Object;
    }
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
    // 子アクターへ owner を渡す（BP の SetOwner）
    if (WeaponChild && WeaponChild->GetChildActor())
    {
        WeaponChild->GetChildActor()->SetOwner(this);
    }
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

void AEnemy::ReceiveAnyDamage(
    float Damage,
    const UDamageType* DamageType,
    AController* InstigatedBy,
    AActor* DamageCauser
)
{
    Super::ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser);

    /* ===========
       Health 減少
       =========== */
    Health -= Damage;

    if (Health > 0.0)
    {
        // 生存時：
    }
    else
    {
        // 死亡アクション
        bIsDead = true;
        PlayDeathAnimation();
    }
}

/* ===========
   ATTACK
 =========== */
void AEnemy::PlayAttackAnimation()
{
    if (!AttackMontage) return;

    float Duration = PlayAnimMontage(AttackMontage);

    if (Duration > 0.0f)
    {
        // Blueprint の Delay の代用
        FTimerHandle Timer;
        GetWorldTimerManager().SetTimer(Timer, [this]()
            {
                // アニメーション停止
                USkeletalMeshComponent* MeshComp = GetMesh();
                if (MeshComp)
                {
                    MeshComp->bPauseAnims = true;

                    // 移動停止
                    if (UCharacterMovementComponent* Move = GetCharacterMovement())
                    {
                        Move->DisableMovement();
                    }
                }

            }, Duration - 0.27f, false); // Blueprint と同じ処理
    }
}

/* ===========
   DEATH
 =========== */
void AEnemy::PlayDeathAnimation()
{
    if (!DeathMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Death montage not assigned!"));
        return;
    }

    // アニメ再生
    float Duration = PlayAnimMontage(DeathMontage);

    if (Duration > 0.f)
    {
        // Blueprint の Delay と同じ挙動
        FTimerHandle Timer;
        GetWorldTimerManager().SetTimer(Timer, [this]()
            {
                USkeletalMeshComponent* MeshComp = GetMesh();
                if (MeshComp)
                {
                    // アニメ停止
                    MeshComp->bPauseAnims = true;
                }

                // 移動停止
                if (UCharacterMovementComponent* Move = GetCharacterMovement())
                {
                    Move->DisableMovement();
                }

                // 必要なら死体のコリジョン無効化
                SetActorEnableCollision(false);

            }, Duration - 0.1f, false); // 少し余裕を持たせて停止
    }
}