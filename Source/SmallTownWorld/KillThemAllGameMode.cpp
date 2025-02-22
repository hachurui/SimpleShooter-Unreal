// Fill out your copyright notice in the Description page of Project Settings.


#include "KillThemAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillThemAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);
        //Attempts to cast the killed pawn's controller as an APlayerController. Calls EndGame(false) (meaning player lost) if it successfully casts
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController != nullptr)
    {
        EndGame(false);
        return; //early return
    }

        //If killed pawn isn't the player, loops through all enemy shooters and calls EndGame(true) if none of them are alive (early return if any of them still live)
    for(AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
    {
        if(AIController->IsDead() == false) return;
    }
    
    EndGame(true);
}



            //this controller is the winner if its value of being the player and value of if the player won is the same
void AKillThemAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}