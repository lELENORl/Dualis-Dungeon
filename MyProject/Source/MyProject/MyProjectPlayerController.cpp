// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MyProjectCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMyProjectPlayerController::AMyProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AMyProjectPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// ���[���h���̑SPawn������
	TArray<AActor*> FoundPawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundPawns);

	for (AActor* Actor : FoundPawns)
	{
		// �Ⴆ�΃^�O�Ŕ��ʁi"Player1", "Player2" �Ȃǂ����O��Pawn�ɐݒ�j
		if (Actor->ActorHasTag("Player1"))
		{
			ControlledPawn1 = Cast<APawn>(Actor);
		}
		else if (Actor->ActorHasTag("Player2"))
		{
			ControlledPawn2 = Cast<APawn>(Actor);
		}
	}
}

void AMyProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events	���N���b�N
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AMyProjectPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AMyProjectPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AMyProjectPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AMyProjectPlayerController::OnSetDestinationReleased);

		// �E�N���b�N�i2�̖ځj
		EnhancedInputComponent->BindAction(SetDestinationRightClickAction, ETriggerEvent::Started, this, &AMyProjectPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationRightClickAction, ETriggerEvent::Triggered, this, &AMyProjectPlayerController::OnSetDestinationRightClickTriggered);
		EnhancedInputComponent->BindAction(SetDestinationRightClickAction, ETriggerEvent::Completed, this, &AMyProjectPlayerController::OnSetDestinationRightClickReleased);
		EnhancedInputComponent->BindAction(SetDestinationRightClickAction, ETriggerEvent::Canceled, this, &AMyProjectPlayerController::OnSetDestinationRightClickReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AMyProjectPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AMyProjectPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AMyProjectPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AMyProjectPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProjectPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AMyProjectPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AMyProjectPlayerController::OnSetDestinationReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		// player1 ��ړI�n�Ɉړ�
		APawn* ControlledPawn = ControlledPawn1 ? ControlledPawn1 : GetPawn();
		if (ControlledPawn && ControlledPawn->GetController())
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(ControlledPawn->GetController(), CachedDestination);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}
	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AMyProjectPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AMyProjectPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

// �E�N���b�N��2�̖ڂ̃L�����𑀍�
void AMyProjectPlayerController::OnSetDestinationRightClickTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	// �q�b�g����i�E�N���b�N�j
	FHitResult Hit;
	bool bHitSuccessful =
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// 2�̖ڂ̃L�����Ɉړ��w��
	if (ControlledPawn2 != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn2->GetActorLocation()).GetSafeNormal();
		ControlledPawn2->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AMyProjectPlayerController::OnSetDestinationRightClickReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		// player2 ��ړI�n�Ɉړ�
		if (ControlledPawn2 && ControlledPawn2->GetController())
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(ControlledPawn2->GetController(), CachedDestination);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor2, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}
	FollowTime = 0.f;
}
