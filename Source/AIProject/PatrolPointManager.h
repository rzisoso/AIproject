// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPointManager.generated.h"

UCLASS()
class AIPROJECT_API APatrolPointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPointManager();

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class USceneComponent* SceneComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (MakeEditWidget = "true"))
	TArray<FVector> PatrolPoints;

	virtual bool ShouldTickIfViewportsOnly() const override;
};
