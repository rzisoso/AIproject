// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_NextSecondPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API UMyBTTask_NextSecondPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	enum class EDirectionType
	{
		Forward,
		Reverse
	};

	EDirectionType DirectionType = EDirectionType::Forward;
};
