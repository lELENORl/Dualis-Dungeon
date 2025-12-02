// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BaseWeapon.generated.h"

UCLASS()
class MYPROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:
    ABaseWeapon();

    // ソード有効化
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwordActive();

    // ソード無効化
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwordDeactive();

    // ソード判定用のコリジョンボックス
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UBoxComponent* SwordCollision;

    UPROPERTY(Category = "Weapon", EditAnywhere)
    UStaticMeshComponent* WeaponBody;

    // ダメージ量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float SwordDamage;

protected:
    virtual void BeginPlay() override;

    // オーバーラップイベント
    UFUNCTION()
    void OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};
