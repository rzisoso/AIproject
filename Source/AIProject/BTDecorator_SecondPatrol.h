// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_SecondPatrol.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API UBTDecorator_SecondPatrol : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsOk = true;
};
