// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"
#include "NPCAIController.h"
#include <Components/SkeletalMeshComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Animation/AnimInstance.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AIPerceptionComponent.h>
#include "AIProjectCHaracter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "SecondPatrolPointManager.h"

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

	SetupPerceptionSystem();

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(this, FName(TEXT("PatrolPoint")), OutActors);
	if (OutActors.Num() != 0)
	{
		MyPatrolPoints = Cast<ASecondPatrolPointManager>(OutActors[0]);
	}
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

void ANPCCharacter::SetupPerceptionSystem()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCCharacter::OnTargetPerceptionChange);
}

void ANPCCharacter::OnTargetPerceptionChange(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionChange"));
	if (Cast<AAIProjectCharacter>(Actor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor"));
		ANPCAIController* CharacterController = Cast<ANPCAIController>(GetController());
		if (CharacterController)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Controller"));
			CharacterController->SetCanSeeKey(Stimulus.WasSuccessfullySensed());
		}
	}
}

ASecondPatrolPointManager* ANPCCharacter::GetCurrentPatrol()
{
	return MyPatrolPoints;
}

