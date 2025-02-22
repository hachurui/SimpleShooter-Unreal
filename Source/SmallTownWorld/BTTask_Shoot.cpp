// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "AIController.h"

#include "PlayerCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
        //Get AIController of owner of this task's owner BT and returns a failure if not accessed
    AAIController* AIPawnController = OwnerComp.GetAIOwner();
    if(!AIPawnController) return EBTNodeResult::Failed;
        //Get Pawn who owns AIController and returns a failure if not accessed
    APlayerCharacter* AIPawn = Cast<APlayerCharacter>(AIPawnController->GetPawn());
    if(!AIPawn) return EBTNodeResult::Failed;
        //Have pawn fire their gun.
    AIPawn->FireFunction();

    return EBTNodeResult::Succeeded;
}