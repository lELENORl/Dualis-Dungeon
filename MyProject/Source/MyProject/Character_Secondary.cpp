// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Secondary.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "FireballProjectile.h" // 新規作成する火球クラスのヘッダ

// Sets default values
ACharacter_Secondary::ACharacter_Secondary()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}
void ACharacter_Secondary::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input の MappingContext を LocalPlayer に追加
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultMappingContext)
				{
					// 優先度は必要に応じて変更
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}
}

void ACharacter_Secondary::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input コンポーネントへバインド
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (CastInputAction)
		{
			// 押した瞬間に呼ぶ（Started）。長押し/離した時は他のトリガーを使う
			EnhancedInput->BindAction(CastInputAction, ETriggerEvent::Started, this, &ACharacter_Secondary::OnCast);
		}
	}
}

void ACharacter_Secondary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_Secondary::OnCast()
{
	// 1) アニメーション再生（あれば）
	if (CastMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(CastMontage);
		}
	}

	// 2) 発射位置と回転（手のソケット等があればそちらを使う）
	FVector SpawnLocation = GetActorLocation() + GetControlRotation().RotateVector(FVector(100.f, 0.f, 50.f));
	FRotator SpawnRotation = GetControlRotation();

	// 3) 火球スポーン
	if (FireballClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AFireballProjectile* Fireball = Cast<AFireballProjectile>(GetWorld()->SpawnActor<AActor>(FireballClass, SpawnLocation, SpawnRotation, SpawnParams));
		if (Fireball)
		{
			// 4) 最寄りの敵を検索してホーミングターゲットに設定
			if (EnemyClass)
			{
				TArray<AActor*> FoundEnemies;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, FoundEnemies);

				AActor* BestTarget = nullptr;
				float BestDistSq = TNumericLimits<float>::Max();
				for (AActor* Candidate : FoundEnemies)
				{
					if (!Candidate || Candidate == this) continue;
					float DistSq = FVector::DistSquared(Candidate->GetActorLocation(), SpawnLocation);
					if (DistSq < BestDistSq)
					{
						BestDistSq = DistSq;
						BestTarget = Candidate;
					}
				}

				if (BestTarget)
				{
					Fireball->SetHomingTarget(BestTarget);
				}
			}
		}
	}
}

