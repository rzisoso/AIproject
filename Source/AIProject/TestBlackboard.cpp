// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBlackboard.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Bool.h>

UTestBlackboard::UTestBlackboard()
{
	Keys.SetNum(1);
	bIsMove = NewObject<UBlackboardKeyType_Bool>();
	if (bIsMove)
	{
		Keys[0].EntryName = TEXT("IsMove");
		Keys[0].KeyType = bIsMove;

	}
}
