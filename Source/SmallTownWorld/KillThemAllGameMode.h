// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmallTownWorldGameMode.h"
#include "KillThemAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SMALLTOWNWORLD_API AKillThemAllGameMode : public ASmallTownWorldGameMode
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);
};
