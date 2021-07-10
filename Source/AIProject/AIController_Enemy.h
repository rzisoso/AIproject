// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIController_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API AAIController_Enemy : public AAIController
{
	GENERATED_BODY()

public:

	AAIController_Enemy();

protected:

	virtual void BeginPlay() override;

	class UBehaviorTree* BTEnemy;

	class UBlackboardComponent* BlackboardComp;

	class UBlackboardData* BlackboardData;

	//EnumClass StateEnum;

	class UUserDefinedEnum* StateEnum;

public:

	UFUNCTION(BlueprintCallable, Category = "State")
	FName GetState();

	UFUNCTION(BlueprintCallable, Category = "State")
	class AAICharacter_Enemy* GetControllerEnemy();

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetStatePassive();

	void SetStateSeeking(FVector TargetLocation);

	void SetStateinvestigate(FVector TargetLocation, FVector POI);

	void SetStateAttack(AActor* Target);

	
};
