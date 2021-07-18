// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateSpeed.h"
#include "NPCAIController.h"
#include "NPCCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

void UBTService_UpdateSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	ANPCAIController* EnemyController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	if (EnemyController)
	{
		ANPCCharacter* Enemy = Cast<ANPCCharacter>(EnemyController->GetPawn());
		if (Enemy)
		{
			Enemy->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}

UBTService_UpdateSpeed::UBTService_UpdateSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("UpdateSpeed");
}