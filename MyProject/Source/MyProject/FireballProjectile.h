// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class MYPROJECT_API AFireballProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireballProjectile();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Visual")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

public:
	// ホーミングターゲットをセット
	UFUNCTION(BlueprintCallable)
	void SetHomingTarget(AActor* Target);

};
