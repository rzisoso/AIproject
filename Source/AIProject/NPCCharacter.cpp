// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"
#include "NPCAIController.h"
#include <Components/SkeletalMeshComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Animation/AnimInstance.h>

// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ANPCAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkeletalMesh(TEXT("/Game/PerceptiveAI/Characters/Mannequin/Mesh/SK_Mannequin"));
	if (TempSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkeletalMesh.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimIns(TEXT("/Game/PerceptiveAI/Characters/Mannequin/Animations/ThirdPerson_AnimBP"));
	if (TempAnimIns.Succeeded())
	{
		GetMesh()->SetAnimClass(TempAnimIns.Class);
	}
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

