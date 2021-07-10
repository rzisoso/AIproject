// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>
#include "AICharacter_Enemy.h"
#include <UObject/ConstructorHelpers.h>
#include "FirstPersonCharacter.h"
#include <Components/CapsuleComponent.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempMontage(TEXT("/Game/Blueprint/AM_Dead"));
	if (TempMontage.Succeeded())
	{
		DeadMontage = TempMontage.Object;
	}
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, Health);
	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %f"), CurrentHealth);
	if (CurrentHealth == 0 && !bIsDead)
	{
		OnDead();
	}
}

void UHealthComponent::OnDead()
{
	AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(GetOwner());
	if (Enemy)
	{
		bIsDead = true;
		Enemy->GetMesh()->SetCollisionProfileName(FName("Ragdoll"), true);
		
		Enemy->GetMesh()->SetAllBodiesSimulatePhysics(true);
		Enemy->GetMesh()->WakeAllRigidBodies();
		Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Enemy->PlayAnimMontage(DeadMontage);
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("DeadDelay");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(this, 2.f, LatentInfo);
		
	}
	AFirstPersonCharacter* Player = Cast<AFirstPersonCharacter>(GetOwner());
	if (Player)
	{
		bIsDead = true;
		Player->Dead();
	}
}

bool UHealthComponent::GetIsDead()
{
	return bIsDead;
}

void UHealthComponent::DeadDelay()
{
	GetOwner()->Destroy();
}

