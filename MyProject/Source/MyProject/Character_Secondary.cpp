// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Secondary.h"

// Sets default values
ACharacter_Secondary::ACharacter_Secondary()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacter_Secondary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Secondary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_Secondary::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

