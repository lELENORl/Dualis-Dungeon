// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Secondary.generated.h"

UCLASS()
class MYPROJECT_API ACharacter_Secondary : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Secondary();

protected:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
