// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();
        //add hud to screen
    HUDScreen = CreateWidget(this, HUDClass);
    if(!HUDScreen) return;
    HUDScreen->AddToViewport();
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
        //Sets time to restart the level
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
        //turns off the HUD
    HUDScreen->RemoveFromParent();

        //Add win screen if player is the winner, lose screen if their the loser.
    if(bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if(!WinScreen) return;

        WinScreen->AddToViewport();
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if(!LoseScreen) return;

        LoseScreen->AddToViewport();
    }
}