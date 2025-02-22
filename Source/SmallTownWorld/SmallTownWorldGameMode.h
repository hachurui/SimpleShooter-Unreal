// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmallTownWorldGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SMALLTOWNWORLD_API ASmallTownWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKilled);
};
