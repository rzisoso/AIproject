// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include <BehaviorTree/BehaviorTree.h>
#include <UObject/ConstructorHelpers.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>

ANPCAIController::ANPCAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> TempBehaviorTree(TEXT("/Game/NPC/AI/BT_NPC"));
	if (TempBehaviorTree.Succeeded())
	{
		NPCBehaviorTree = TempBehaviorTree.Object;
	}

	//BTComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComp"));
	//BTComp->Tree
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();
	if (NPCBehaviorTree)
	{
		RunBehaviorTree(NPCBehaviorTree);
	}
}

//void ANPCAIController::OnPossess(APawn* InPawn)
//{
//	Super::OnPossess(InPawn);
//	if (BTComp && NPCBehaviorTree)
//	{
//		BTComp->StartTree(*NPCBehaviorTree);
//	}
//}
