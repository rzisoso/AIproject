// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"
#include <Components/SkeletalMeshComponent.h>
#include <Camera/CameraComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Engine/SkeletalMesh.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>
#include "Bullet.h"
#include <Sound/SoundWave.h>
#include <Kismet/GameplayStatics.h>
#include "HealthComponent.h"
#include "AnimInstance_Enemy.h"
#include <Curves/CurveFloat.h>

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetRootComponent());

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCamera);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh1P(TEXT("/Game/PerceptiveAI/Characters/FPP/Character/Mesh/SK_Mannequin_Arms"));
	if (TempMesh1P.Succeeded())
	{
		Mesh1P->SetSkeletalMesh(TempMesh1P.Object);
	}

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	EAttachmentRule Rules = EAttachmentRule::SnapToTarget;
	FAttachmentTransformRules AttachmentRules(Rules, true);

	FP_Gun->AttachToComponent(Mesh1P, AttachmentRules, FName("GripPoint"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Game/PerceptiveAI/Characters/FPP/FPWeapon/Mesh/SK_FPGun"));
	if (TempGunMesh.Succeeded())
	{
		FP_Gun->SetSkeletalMesh(TempGunMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TempFireMontage(TEXT("/Game/Blueprint/AM_Fire"));
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

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> TempCurve(TEXT("/Game/Blueprint/PostCurve"));
	if (TempCurve.Succeeded())
	{
		PostCurve = TempCurve.Object;
	}

	PostTimeline = FTimeline{};
	
	FirstPersonCamera->PostProcessSettings.bOverride_ColorSaturation = true;
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FOnTimelineFloatStatic InterpFunc;
	InterpFunc.BindUFunction(this, FName("PostFunc"));
	PostTimeline.AddInterpFloat(PostCurve, InterpFunc);
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PostTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFirstPersonCharacter::Jump);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFirstPersonCharacter::MoveRight); 
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFirstPersonCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFirstPersonCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AFirstPersonCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AFirstPersonCharacter::EndFire);
}

void AFirstPersonCharacter::Jump()
{
	Super::Jump();
}

void AFirstPersonCharacter::MoveForward(float Val)
{
	AddMovementInput(GetActorForwardVector(), Val);
}

void AFirstPersonCharacter::MoveRight(float Val)
{
	AddMovementInput(GetActorRightVector(), Val);
}

void AFirstPersonCharacter::Turn(float Val)
{
	AddControllerYawInput(Val);
}

void AFirstPersonCharacter::LookUp(float Val)
{
	AddControllerPitchInput(Val);
}

void AFirstPersonCharacter::StartFire()
{
	bIsFire = true;
	Fire();
}

void AFirstPersonCharacter::EndFire()
{
	bIsFire = false;
}

bool AFirstPersonCharacter::GetIsFire()
{
	return bIsFire;
}

void AFirstPersonCharacter::Fire()
{
	
	UAnimInstance* AnimIns = Mesh1P->GetAnimInstance();
	if (AnimIns)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Fire"));
		AnimIns->Montage_Play(FireMontage, 5.f);
		UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, GetActorLocation(), 0.2f);
		FRotator ViewRotation = GetViewRotation();
		FVector Forward = FP_Gun->GetSocketLocation(FName("Muzzle")) - FP_Gun->GetSocketLocation(FName("Start"));
		FRotator ForwardRotation = Forward.Rotation();
		FVector PointLocation = FP_Gun->GetSocketLocation(FName("Muzzle"));
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABullet>(BulletClass, PointLocation, ForwardRotation, SpawnParameters);

		MakeNoise(1.f, this, GetActorLocation(), 0.f, FName("GunShot"));
	}
}

int8 AFirstPersonCharacter::GetTeamID()
{
	return 1;
}

bool AFirstPersonCharacter::GetisDead()
{
	return HealthComp->GetIsDead();
}

void AFirstPersonCharacter::NotifyStateChanged(FName StateName)
{
	EnumName = StateName;
	UAnimInstance_Enemy* AnimInstance = Cast<UAnimInstance_Enemy>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnStateChanged(StateName);
	}
}

bool AFirstPersonCharacter::GetIsDeadBlue()
{
	return GetisDead();
}

void AFirstPersonCharacter::Dead()
{
	//UE_LOG(LogTemp, Warning, TEXT("Dead"));
	PostTimeline.PlayFromStart();
}

void AFirstPersonCharacter::PostFunc(float Val)
{
	//UE_LOG(LogTemp, Warning, TEXT("Post: %f"), PostCurve->GetFloatValue(Val));
	PostVal = FMath::Lerp(1.f, 0.f, PostCurve->GetFloatValue(Val));
	//UE_LOG(LogTemp, Warning, TEXT("PostVal: %f"), PostVal);
	FirstPersonCamera->PostProcessSettings.ColorSaturation.Set(PostVal, PostVal, PostVal, PostVal);
}

