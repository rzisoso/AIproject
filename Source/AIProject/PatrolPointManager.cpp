// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPointManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/SceneComponent.h>

// Sets default values
APatrolPointManager::APatrolPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = WITH_EDITOR;

	//PatrolPoints.Widget

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	PatrolPoints.Add(FVector(10.f, 10.f, 0.f));
	PatrolPoints.Add(FVector(499.f, 30.f, 0.f));
	PatrolPoints.Add(FVector(1311.f, 774.f, 0.f));

	
}

// Called when the game starts or when spawned
void APatrolPointManager::BeginPlay()
{
	Super::BeginPlay();
	

	
}

// Called every frame
void APatrolPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UKismetSystemLibrary::FlushPersistentDebugLines(this);
	for (size_t i = 0; i < PatrolPoints.Num() - 1; i++)
	{
		FVector StartPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoints[i]);
		FVector EndPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), PatrolPoints[i + 1]);
		UKismetSystemLibrary::DrawDebugLine(this, StartPoint, EndPoint, FLinearColor::Blue, 10.f, 10.f);
	}
}

bool APatrolPointManager::ShouldTickIfViewportsOnly() const
{
	return WITH_EDITOR;
}

