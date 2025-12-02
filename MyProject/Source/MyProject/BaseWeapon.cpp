// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// コンストラクタ
ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    // SceneRoot（親）作成
    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = SceneRoot;

    // SwordCollision（子）作成
    SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
    SwordCollision->SetupAttachment(RootComponent); // SceneRootの下

    WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBody"));
    WeaponBody->SetupAttachment(RootComponent); // SceneRootの下

    SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SwordCollision->SetCollisionProfileName(TEXT("OverlapAll"));

    SwordDamage = 10.0f; // 初期ダメージ値
}

// BeginPlay（イベント登録）
void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    // オーバーラップイベント登録
    if (SwordCollision)
    {
        SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnSwordBeginOverlap);
    }
}

// オーバーラップイベント本体
void ABaseWeapon::OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // 自身とプレイヤー判定
    AActor* MyOwner = GetOwner();
    if (OtherActor && OtherActor != MyOwner)
    {
        // ダメージを付与
        UGameplayStatics::ApplyDamage(
            OtherActor,
            SwordDamage,
            nullptr,       // Instigator
            this,          // Damage Causer
            nullptr        // DamageTypeClass
        );
    }
}

// 「攻撃開始」- ソードを有効化（コリジョンON）
void ABaseWeapon::SwordActive()
{
    if (SwordCollision)
    {
        SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SwordCollision->SetCollisionProfileName(TEXT("OverlapAll"), true);
    }
}

// 「攻撃終了」- ソード無効化（コリジョンOFF）
void ABaseWeapon::SwordDeactive()
{
    if (SwordCollision)
    {
        SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

