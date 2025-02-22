// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior)
    {
        RunBehaviorTree(AIBehavior);
        PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        // GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if(!PlayerPawn) return;
    if(LineOfSightTo(PlayerPawn))
    {
        //UE_LOG(LogTemp, Warning, TEXT("Has Sight"));
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
        //SetFocus(PlayerPawn);
    }
    else 
    {
        //UE_LOG(LogTemp, Warning, TEXT("Lost Sight"));
        //ClearFocus(EAIFocusPriority::Gameplay);
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }
}

bool AShooterAIController::IsDead() const
{
    APlayerCharacter* ControlledCharacter = Cast<APlayerCharacter>(GetPawn());
    if(ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}