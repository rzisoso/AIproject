// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FindPlayerLocation.h"
#include <Kismet/GameplayStatics.h>
#include "AIProjectCharacter.h"
#include "NPCAIController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "./../../Intermediate/ProjectFiles/BBKey.h"

EBTNodeResult::Type UMyBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIProjectCharacter* Player = Cast<AAIProjectCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKeys::TargetLocation, PlayerLocation);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
