// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter_Enemy.h"
#include <UObject/ConstructorHelpers.h>
#include <Animation/AnimInstance.h>
#include "AIController_Enemy.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "HealthComponent.h"
#include "AnimInstance_Enemy.h"
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AISenseConfig_Hearing.h>
#include <Perception/AIPerceptionSystem.h>
#include <Kismet/KismetSystemLibrary.h>
#include "FirstPersonCharacter.h"
#include <GameFramework/PlayerController.h>
#include <NavigationSystem.h>
#include <Kismet/GameplayStatics.h>
#include "PatrolPointManager.h"
#include <Perception/AISense_Sight.h>
#include <Perception/AISense_Hearing.h>
#include <Animation/AnimMontage.h>
#include "Bullet.h"
#include <Sound/SoundWave.h>
#include <TimerManager.h>
//#include "Perception/AIPerceptionTypes.h"
//#include <Engine/UserDefinedEnum.h>

// Sets default values
AAICharacter_Enemy::AAICharacter_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Game/PerceptiveAI/Characters/Mannequin/Mesh/SK_Mannequin"));
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

	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	EAttachmentRule AttachmentRule = EAttachmentRule::SnapToTarget;
	FAttachmentTransformRules AttachmentRules(AttachmentRule, AttachmentRule, AttachmentRule, true);

	WeaponComp->AttachToComponent(GetMesh(), AttachmentRules, FName("GripPoint"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempWeapon(TEXT("/Game/PerceptiveAI/Characters/FPP/FPWeapon/Mesh/SK_FPGun"));
	if (TempWeapon.Succeeded())
	{
		WeaponComp->SetSkeletalMesh(TempWeapon.Object);
	}

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));

	if (PerceptionComp)
	{
		SenseConfig_Sight->SightRadius = 1200.f;
		SenseConfig_Sight->LoseSightRadius = 2000.f;
		SenseConfig_Sight->PeripheralVisionAngleDegrees = 130.f;
		SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
		SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
		SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
		SenseConfig_Sight->SetMaxAge(3.f);
		PerceptionComp->ConfigureSense(*SenseConfig_Sight);
	}

	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));

	if (PerceptionComp)
	{
		SenseConfig_Hearing->HearingRange = 2500.f;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = true;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = true;
		SenseConfig_Hearing->SetMaxAge(3.f);
		PerceptionComp->ConfigureSense(*SenseConfig_Hearing);
		PerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	}

	//FObjectInitializer
	//SenseConfig_Sight = UAISenseConfig_Sight();
	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempFireMontage(TEXT("/Game/Blueprint/AM_Fire_Enemy"));
	if (TempFireMontage.Succeeded())
	{
		FireMontage = TempFireMontage.Object;
	}

	static ConstructorHelpers::FClassFinder<ABullet> TempBulletClass(TEXT("/Game/Blueprint/BP_Bullet"));
	if (TempBulletClass.Succeeded())
	{
		BulletClass = TempBulletClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> TempSound(TEXT("/Game/PerceptiveAI/Characters/FPP/Audio/FirstPersonTemplateWeaponFire02"));
	if (TempSound.Succeeded())
	{
		WeaponSound = TempSound.Object;
	}
}

// Called when the game starts or when spawned
void AAICharacter_Enemy::BeginPlay()
{
	Super::BeginPlay();

	//if (SenseConfig_Sight.IsValidLowLevel())
	//{
	//	SenseConfig_Sight.SightRadius = 1200.f;
	//	SenseConfig_Sight.LoseSightRadius = 2000.f;
	//	SenseConfig_Sight.PeripheralVisionAngleDegrees = 130.f;
	//	SenseConfig_Sight.DetectionByAffiliation.bDetectEnemies = true;
	//	SenseConfig_Sight.DetectionByAffiliation.bDetectNeutrals = true;
	//	SenseConfig_Sight.DetectionByAffiliation.bDetectFriendlies = true;
	//	SenseConfig_Sight.SetMaxAge(3.f);
	//	PerceptionComp->ConfigureSense(SenseConfig_Sight);
	//}
	if (PerceptionComp)
	{
		PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAICharacter_Enemy::OnPerceptionChanged);
	}
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Patrol_1")), OutActors);
	if (OutActors.Num() != 0)
	{
		MyPatrol = Cast<APatrolPointManager>(OutActors[0]);
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *MyPatrol->GetName());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Noll"));
	}
	
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

void AAICharacter_Enemy::UpdateSpeed(int Num)
{
	if (Num == 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if (Num == 1)
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if (Num == 2)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
	else if (Num == 3)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
	
}

int8 AAICharacter_Enemy::GetTeamID()
{
	return 0;
}

bool AAICharacter_Enemy::GetisDead()
{
	return HealthComp->GetIsDead();
}

void AAICharacter_Enemy::NotifyStateChanged(FName StateName)
{
	EnumName = StateName;
	UAnimInstance_Enemy* AnimInstance = Cast<UAnimInstance_Enemy>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnStateChanged(StateName);
	}
}

APatrolPointManager* AAICharacter_Enemy::GetPatrol()
{
	return MyPatrol;
}

void AAICharacter_Enemy::SetAnimMode(bool IsWarning)
{
	UAnimInstance_Enemy* AnimIns = Cast<UAnimInstance_Enemy>(GetMesh()->GetAnimInstance());
	if (AnimIns)
	{
		//AnimIns->SetWarning(IsWarning);
		bIsWarning = IsWarning;
	}
}

float AAICharacter_Enemy::GetAttackRange()
{
	return 800.0f;
}

void AAICharacter_Enemy::OnPerceptionChanged(const TArray<AActor*>& UpdatedActors)
{
	
	for (auto Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		PerceptionComp->GetActorsPerception(Actor, Info);
		LastStimuliActor = Info.Target;
		for (auto LastSensed : Info.LastSensedStimuli)
		{
			LastStimuliSensed = LastSensed;
			TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, LastStimuliSensed);
			//UE_LOG(LogTemp, Warning, TEXT("Class: %s"), *SenseClass.Get()->GetFName().ToString());
			/*if (SenseClass.Get()->IsValidLowLevel())
			{
				UE_LOG(LogTemp, Warning, TEXT("Class: %s"), *SenseClass.Get()->GetFName().ToString());
				UE_LOG(LogTemp, Warning, TEXT("Class: %s"), *UAISenseConfig_Sight::StaticClass()->GetFName().ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("NotValid"));
			}*/
			if (SenseClass.Get() == UAISense_Sight::StaticClass())
			{
				UE_LOG(LogTemp, Warning, TEXT("Sight"));
				PocessLastVision();
			}
			else if (SenseClass.Get() == UAISense_Hearing::StaticClass())
			{
				UE_LOG(LogTemp, Warning, TEXT("Hear"));
				PocessLastHearing();
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("None"));
			}

		}
	}
}

void AAICharacter_Enemy::PocessLastVision()
{
	bool bIsInterface = UKismetSystemLibrary::DoesImplementInterface(LastStimuliActor, UInterface_AIHelper::StaticClass());
	if (bIsInterface)
	{
		IInterface_AIHelper* HasInterface = Cast<IInterface_AIHelper>(LastStimuliActor);
		if (HasInterface)
		{
			int TeamID =  HasInterface->GetTeamID();
			if (TeamID)
			{
				AFirstPersonCharacter* Player = Cast<AFirstPersonCharacter>(LastStimuliActor);
				if (Player)
				{
					bool bIsDead = Player->GetisDead();
					if (!bIsDead)
					{
						AAIController_Enemy* Controller_Enemy = Cast<AAIController_Enemy>(GetController());
						if (Controller_Enemy)
						{
							Controller_Enemy->SetStateAttack(LastStimuliActor);
						}
					}
				}
			}
			else
			{
				AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(LastStimuliActor);
				if (Enemy)
				{
					bool bIsDead = Enemy->GetisDead();
					if (bIsDead)
					{
						AAIController_Enemy* AI_Enemy = Cast<AAIController_Enemy>(GetController());
						if (AI_Enemy)
						{
							AI_Enemy->SetStateSeeking(LastStimuliSensed.StimulusLocation);
						}
					}
				}
			}
		}
	}
}

void AAICharacter_Enemy::PocessLastHearing()
{
	if (LastStimuliSensed.Tag == FName("Alert") || LastStimuliSensed.Tag == FName("GunShot"))
	{
		if (LastStimuliSensed.WasSuccessfullySensed())
		{
			FNavLocation OutLocation;
			UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(this);
			NavigationSystem->ProjectPointToNavigation(LastStimuliSensed.StimulusLocation, OutLocation, FVector(50.f, 50.f, 200.f));
			AAIController_Enemy* Controller_Enemy = Cast<AAIController_Enemy>(GetController());
			if (Controller_Enemy)
			{
				AAIController_Enemy* EnemyController = Cast<AAIController_Enemy>(GetController());
				if (EnemyController)
				{
					Controller_Enemy->SetStateSeeking(OutLocation);
				}

			}
		}
		
	}
	else
	{
		if (LastStimuliSensed.WasSuccessfullySensed())
		{
			FNavLocation OutLocation;
			UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(this);
			NavigationSystem->ProjectPointToNavigation(LastStimuliSensed.StimulusLocation, OutLocation, FVector(50.f, 50.f, 200.f));
			AAIController_Enemy* Controller_Enemy = Cast<AAIController_Enemy>(GetController());
			if (Controller_Enemy)
			{
				UE_LOG(LogTemp, Warning, TEXT("SetState"));
				Controller_Enemy->SetStateinvestigate(OutLocation, LastStimuliSensed.StimulusLocation);
			}
		}
		
	}
	
}

APatrolPointManager* AAICharacter_Enemy::GetCurrentPatrol()
{
	return GetPatrol();
}

void AAICharacter_Enemy::SetAnimModeBlue(bool IsWarning)
{
	SetAnimMode(IsWarning);
}

float AAICharacter_Enemy::GetAttackRangeBlue()
{
	return GetAttackRange();
}

//FName AAICharacter_Enemy::GetStateName()
//{
//	return EnumName;
//}

void AAICharacter_Enemy::StartFire()
{
	UE_LOG(LogTemp, Warning, TEXT("StartFire"));
	if (bIsAutoFire)
	{
		//bIsFireing = true;
		UE_LOG(LogTemp, Warning, TEXT("true"));
		GetWorldTimerManager().SetTimer(FireHandle, FTimerDelegate::CreateLambda([this]() {this->Fire(); }), 0.1f, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("false"));
		Fire();
		Fire_Broadcast.Broadcast();
	}
}

void AAICharacter_Enemy::EndFire()
{
	GetWorldTimerManager().ClearTimer(FireHandle);
	Fire_Broadcast.Broadcast();
}

//bool AAICharacter_Enemy::GetIsFire()
//{
//	return bIsFireing;
//}

void AAICharacter_Enemy::Fire()
{
	if (CurrentBulletCount < MaxBulletCount)
	{
		++CurrentBulletCount;
		UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
		if (AnimIns)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Fire"));
			AnimIns->Montage_Play(FireMontage, 5.f);
			UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, GetActorLocation(), 0.2f);
			FRotator ViewRotation = GetViewRotation();
			FVector Forward = WeaponComp->GetSocketLocation(FName("Muzzle")) - WeaponComp->GetSocketLocation(FName("Start"));
			FRotator ForwardRotation = Forward.Rotation();
			FVector PointLocation = WeaponComp->GetSocketLocation(FName("Muzzle"));
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<ABullet>(BulletClass, PointLocation, ForwardRotation, SpawnParameters);
		}
	}
	else
	{
		CurrentBulletCount = 0;
		EndFire();
	}
	
}

