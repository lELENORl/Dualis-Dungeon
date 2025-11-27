// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class MYPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

protected:

    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator,
        AActor* DamageCauser
    ) override;

    UPROPERTY(Category = "UnitComponent", EditAnywhere)
    UStaticMeshComponent* WeaponMesh;

    /* =======================
       VARIABLES
       ======================= */

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob")
    double Health = 100.0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mob")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mob")
    UAnimMontage* DeathMontage;

    bool bIsDead = false; 

    /* =======================
       CHILD ACTOR WEAPON
       ======================= */

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<AActor> WeaponClass;

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    //class UChildActorComponent* WeaponChild;


    /* =======================
       FUNCTIONS
       ======================= */

    void PlayAttackAnimation();
    void PlayDeathAnimation();
};
