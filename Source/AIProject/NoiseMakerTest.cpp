// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseMakerTest.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <UObject/ConstructorHelpers.h>
#include <Components/SphereComponent.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ANoiseMakerTest::ANoiseMakerTest()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TempParticle(TEXT("/Game/StarterContent/Particles/P_Explosion"));
	if (TempParticle.Succeeded())
	{
		NoiseParticle = TempParticle.Object;
	}
}

// Called when the game starts or when spawned
void ANoiseMakerTest::BeginPlay()
{
	Super::BeginPlay();
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("BeginMakeNoise");
	LatentInfo.Linkage = __LINE__;
	LatentInfo.UUID = 1001;
	UKismetSystemLibrary::Delay(this, 4.f, LatentInfo);

	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ANoiseMakerTest::Stop);
}

// Called every frame
void ANoiseMakerTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), 20.f, 12, FLinearColor::Yellow, 1.f, 10.f);
}

// Called to bind functionality to input
void ANoiseMakerTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANoiseMakerTest::BeginMakeNoise()
{
	if (!bIsSeeking)
	{
		MakeNoise(1.f, this, GetActorLocation(), 0.f, FName("None"));
		UGameplayStatics::SpawnEmitterAtLocation(this, NoiseParticle, GetActorLocation(), FRotator::ZeroRotator, true);
		/*FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("Stop");
		LatentInfo.Linkage = __LINE__;
		LatentInfo.UUID = 1002;
		UKismetSystemLibrary::Delay(this, 3.f, LatentInfo);*/
	}
	else
	{
		MakeNoise(1.f, this, GetActorLocation(), 0.f, FName("Alert"));
		UGameplayStatics::SpawnEmitterAtLocation(this, NoiseParticle, GetActorLocation(), FRotator::ZeroRotator, true);
		/*FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("Stop");
		LatentInfo.Linkage = __LINE__;
		LatentInfo.UUID = 1003;
		UKismetSystemLibrary::Delay(this, 3.f, LatentInfo);*/
	}
	
}

void ANoiseMakerTest::Stop()
{
	
	Destroy();
}

//bool ANoiseMakerTest::ShouldTickIfViewportsOnly() const
//{
//	return WITH_EDITOR;
//}

