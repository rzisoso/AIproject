// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NoiseMakerTest.generated.h"

UCLASS()
class AIPROJECT_API ANoiseMakerTest : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANoiseMakerTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsSeeking;

	class UParticleSystem* NoiseParticle;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class USphereComponent* SphereComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BeginMakeNoise();

	UFUNCTION()
	void Stop();

	//virtual bool ShouldTickIfViewportsOnly() const override;
};
