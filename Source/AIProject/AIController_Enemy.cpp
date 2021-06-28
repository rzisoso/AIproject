// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Enemy.h"
#include <BehaviorTree/BehaviorTree.h>
#include <UObject/ConstructorHelpers.h>
#include <BehaviorTree/BlackboardComponent.h>

AAIController_Enemy::AAIController_Enemy()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> TempBT(TEXT("/Game/AI/BasicTutorial/BehaviorTree/BT_Enemy"));
	if (TempBT.Succeeded())
	{
		BTEnemy = TempBT.Object;
	}
}

void AAIController_Enemy::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BTEnemy);
	/*UBlackboardComponent* BlcakboardComp = GetBlackboardComponent();

	BlcakboardComp->SetValueAsVector(FName("Destination"), FVector(-770.002563, 1190.000000, 226.001602));*/
	
}



