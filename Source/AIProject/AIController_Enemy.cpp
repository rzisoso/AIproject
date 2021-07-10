// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Enemy.h"
#include <BehaviorTree/BehaviorTree.h>
#include <UObject/ConstructorHelpers.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Engine/UserDefinedEnum.h>
#include <UObject/Class.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BlackboardData.h>
#include "AICharacter_Enemy.h"

AAIController_Enemy::AAIController_Enemy()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> TempBT(TEXT("/Game/AI/AI_Enemy/BT_AIEnemy"));
	if (TempBT.Succeeded())
	{
		BTEnemy = TempBT.Object;
	}

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	static ConstructorHelpers::FObjectFinder<UBlackboardData> TempData(TEXT("/Game/AI/AI_Enemy/BB_AIEnemy"));
	if (TempData.Succeeded())
	{
		BlackboardData = TempData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UUserDefinedEnum> TempEnum(TEXT("/Game/AI/AI_Enemy/EEnemyState"));
	if (TempEnum.Succeeded())
	{
		StateEnum = TempEnum.Object;
	}
}

void AAIController_Enemy::BeginPlay()
{
	Super::BeginPlay();

	UseBlackboard(BlackboardData, BlackboardComp);

	RunBehaviorTree(BTEnemy);
	/*UBlackboardComponent* BlcakboardComp = GetBlackboardComponent();

	BlcakboardComp->SetValueAsVector(FName("Destination"), FVector(-770.002563, 1190.000000, 226.001602));*/
	
	//EnumClass StateEnum;

	//UE_LOG(LogTemp, Warning, TEXT("Enum: %s"), *EnumClass->GetAuthoredNameStringByIndex(0));
	//StateEnum->
	//GetState();
}

FName AAIController_Enemy::GetState()
{
	uint8 Val = BlackboardComp->GetValueAsEnum(FName("EnemyState"));
	FName TempName = FName(*StateEnum->GetAuthoredNameStringByValue(Val));
	//UE_LOG(LogTemp, Warning, TEXT("STateName: %s"), *(TempName.ToString()));
	return TempName;
}

AAICharacter_Enemy* AAIController_Enemy::GetControllerEnemy()
{
	AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(GetPawn());
	
	return Enemy;
}

void AAIController_Enemy::SetStatePassive()
{
	BlackboardComp->SetValueAsEnum(FName("EnemyState"), 0);
	AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(GetPawn());
	if (Enemy)
	{
		uint8 Val = BlackboardComp->GetValueAsEnum(FName("EnemyState"));
		FName TempName = FName(*StateEnum->GetAuthoredNameStringByValue(Val));
		//UE_LOG(LogTemp, Warning, TEXT("STateName: %s"), *(TempName.ToString()));
		Enemy->NotifyStateChanged(TempName);
	}
}

void AAIController_Enemy::SetStateSeeking(FVector TargetLocation)
{
	BlackboardComp->SetValueAsEnum(FName("EnemyState"), 2);
	BlackboardComp->SetValueAsVector(FName("Targetlocation"), TargetLocation);
	AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(GetPawn());
	if (Enemy)
	{
		uint8 Val = BlackboardComp->GetValueAsEnum(FName("EnemyState"));
		FName TempName = FName(*StateEnum->GetAuthoredNameStringByValue(Val));
		//UE_LOG(LogTemp, Warning, TEXT("STateName: %s"), *(TempName.ToString()));
		Enemy->NotifyStateChanged(TempName);
	}
}

void AAIController_Enemy::SetStateinvestigate(FVector TargetLocation, FVector POI)
{
	BlackboardComp->SetValueAsEnum(FName("EnemyState"), 1);
	BlackboardComp->SetValueAsVector(FName("Targetlocation"), TargetLocation);
	BlackboardComp->SetValueAsVector(FName("POI"), POI);
	AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(GetPawn());
	if (Enemy)
	{
		uint8 Val = BlackboardComp->GetValueAsEnum(FName("EnemyState"));
		FName TempName = FName(*StateEnum->GetAuthoredNameStringByValue(Val));
		//UE_LOG(LogTemp, Warning, TEXT("STateName: %s"), *(TempName.ToString()));
		Enemy->NotifyStateChanged(TempName);
	}
}

void AAIController_Enemy::SetStateAttack(AActor* Target)
{
	BlackboardComp->SetValueAsEnum(FName("EnemyState"), 3);
	BlackboardComp->SetValueAsObject(FName("PlayerTarget"), Target);
	AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(GetPawn());
	if (Enemy)
	{
		uint8 Val = BlackboardComp->GetValueAsEnum(FName("EnemyState"));
		FName TempName = FName(*StateEnum->GetAuthoredNameStringByValue(Val));
		//UE_LOG(LogTemp, Warning, TEXT("STateName: %s"), *(TempName.ToString()));
		Enemy->NotifyStateChanged(TempName);
	}
}





