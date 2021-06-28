// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter_Enemy.h"
#include <UObject/ConstructorHelpers.h>
#include <Animation/AnimInstance.h>
#include "AIController_Enemy.h"

// Sets default values
AAICharacter_Enemy::AAICharacter_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController_Enemy::StaticClass();

	static ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("/Game/AI/BasicTutorial/Enemy/ABP_Enemy"));
	if (TempAnim.Succeeded())
	{
		EnemyAnim = TempAnim.Class;
		GetMesh()->AnimClass = EnemyAnim;
	}
}

// Called when the game starts or when spawned
void AAICharacter_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAICharacter_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

