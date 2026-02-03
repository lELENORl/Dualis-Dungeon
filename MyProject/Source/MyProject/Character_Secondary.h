// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Character_Secondary.generated.h"

UCLASS()
class MYPROJECT_API ACharacter_Secondary : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacter_Secondary();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Enhanced Input アセット（Editorでセット）
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CastInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	// 発射処理（ボタン押下時）
	UFUNCTION()
	void OnCast();

	// Combat / Projectile 関連プロパティ
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AActor> FireballClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* CastMontage;

	// 最寄りの敵を探すクラス（Editorで敵クラスをセット）
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AActor> EnemyClass;

};
