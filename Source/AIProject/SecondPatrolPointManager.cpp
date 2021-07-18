// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondPatrolPointManager.h"
#include <Components/SceneComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ASecondPatrolPointManager::ASecondPatrolPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = WITH_EDITOR;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);
}

// Called when the game starts or when spawned
void ASecondPatrolPointManager::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ASecondPatrolPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UKismetSystemLibrary::FlushPersistentDebugLines(this);
	/*for (size_t i = 0; i < PatrolPoints.Num() - 1; i++)
	{
		FVector StartPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoints[i]);
		FVector EndPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoints[i + 1]);
		UKismetSystemLibrary::DrawDebugLine(this, StartPoint, EndPoint, FLinearColor::Red, 1.f, 10.f);
	}*/
}

bool ASecondPatrolPointManager::ShouldTickIfViewportsOnly() const
{
	return WITH_EDITOR;
}

