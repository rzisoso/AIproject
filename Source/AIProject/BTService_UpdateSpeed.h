// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateSpeed.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API UBTService_UpdateSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_UpdateSpeed();
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Speed = 600.f;
};
