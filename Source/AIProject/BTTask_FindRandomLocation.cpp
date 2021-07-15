// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include <NavigationSystem.h>
#include "NPCAIController.h"
#include "NPCCharacter.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("FindRandomPoint");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("ExcuteTask"));
	ANPCAIController* NPCController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	if (NPCController)
	{
		ANPCCharacter* Enemy = Cast<ANPCCharacter>(NPCController->GetPawn());
		if (Enemy)
		{
			UNavigationSystemV1* CurrentNavigation = UNavigationSystemV1::GetCurrent(this);
			if (CurrentNavigation)
			{
				//FVector Point = CurrentNavigation->GetRandomPointInNavigableRadius(this, Enemy->GetActorLocation(), Radius);
				FVector Point =UNavigationSystemV1::GetRandomPointInNavigableRadius(this, Enemy->GetActorLocation(), Radius);
				//UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f, Z: %f"), Point.X, Point.Y, Point.Z);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), Point);
				return EBTNodeResult::Succeeded;
			}
			
		}
	}
	
	return EBTNodeResult::Type();
}
