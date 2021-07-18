// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_NextSecondPatrolPoint.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "./../../Intermediate/ProjectFiles/BBKey.h"
#include "NPCAIController.h"
#include "NPCCharacter.h"
#include "SecondPatrolPointManager.h"

EBTNodeResult::Type UMyBTTask_NextSecondPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int CurrentIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKeys::CurrentPoint);
	ANPCAIController* EnemyController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	if (EnemyController)
	{
		ANPCCharacter* Enemy = Cast<ANPCCharacter>(EnemyController->GetPawn());
		if (Enemy)
		{
			int PointsNum = Enemy->GetCurrentPatrol()->PatrolPoints.Num();
			int MinIndex = 0, MaxIndex = PointsNum - 1;
			if (CurrentIndex >= MaxIndex)
			{
				DirectionType = EDirectionType::Reverse;
			}
			else if (CurrentIndex <= MinIndex)
			{
				DirectionType = EDirectionType::Forward;
			}

			//CurrentIndex = (CurrentIndex + 1) % PointsNum;
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKeys::CurrentPoint, (DirectionType == EDirectionType::Forward ? ++CurrentIndex : --CurrentIndex));
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
