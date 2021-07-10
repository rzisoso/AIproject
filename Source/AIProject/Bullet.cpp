// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <UObject/ConstructorHelpers.h>
#include "FirstPersonCharacter.h"
#include "AICharacter_Enemy.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Game/PerceptiveAI/Characters/FPP/Meshes/FirstPersonProjectileMesh"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnSphereBeginOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetOwner() && (GetActorLocation() - GetOwner()->GetActorLocation()).Size() > 20000.f)
	{
		Destroy();
	}
}

void ABullet::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *OtherActor->GetClass()->GetName());
	if (OtherActor->GetClass() != GetClass() && OtherActor != GetOwner())
	{
		AFirstPersonCharacter* Player = Cast<AFirstPersonCharacter>(OtherActor);
		if (Player)
		{
			UGameplayStatics::ApplyPointDamage(Player, 10.f, Player->GetActorLocation() - GetActorLocation(), SweepResult, UGameplayStatics::GetPlayerController(this, 0), this, UDamageType::StaticClass());
		}
		AAICharacter_Enemy* Enemy = Cast<AAICharacter_Enemy>(OtherActor);
		if (Enemy)
		{
			UGameplayStatics::ApplyPointDamage(Enemy, 10.f, Enemy->GetActorLocation() - GetActorLocation(), SweepResult, Enemy->GetController(), this, UDamageType::StaticClass());
		}
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *OtherActor->GetClass()->GetName());
		Destroy();
	}
	
}

