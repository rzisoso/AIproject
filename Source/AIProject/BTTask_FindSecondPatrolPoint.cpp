// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindSecondPatrolPoint.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "./../../Intermediate/ProjectFiles/BBKey.h"
#include "NPCAIController.h"
#include "NPCCharacter.h"
#include "SecondPatrolPointManager.h"
#include <Kismet/KismetMathLibrary.h>

EBTNodeResult::Type UBTTask_FindSecondPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int PointIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKeys::CurrentPoint);
	ANPCAIController* EnemyController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	if (EnemyController)
	{
		ANPCCharacter* EnemyCharacter = Cast<ANPCCharacter>(EnemyController->GetPawn());
		if (EnemyCharacter)
		{
			ASecondPatrolPointManager* PatrolPoints = EnemyCharacter->GetCurrentPatrol();
			if (PatrolPoints)
			{
				FVector PointLocation = PatrolPoints->PatrolPoints[PointIndex];
				PointLocation = PatrolPoints->GetActorTransform().TransformPosition(PointLocation);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKeys::PointLocation, PointLocation);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
