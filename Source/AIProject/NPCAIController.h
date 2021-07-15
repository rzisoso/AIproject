// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API ANPCAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ANPCAIController();

protected:

	virtual void BeginPlay() override;

	//virtual void OnPossess(APawn* InPawn) override;

	class UBehaviorTree* NPCBehaviorTree;

	//class UBehaviorTreeComponent* BTComp;

	class UBlackboardComponent* BlackboardComp;

};
