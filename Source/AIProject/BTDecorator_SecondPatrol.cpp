// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_SecondPatrol.h"

bool UBTDecorator_SecondPatrol::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	return bIsOk;
}
