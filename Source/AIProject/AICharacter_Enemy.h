// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface_AIHelper.h"
#include "Perception/AIPerceptionTypes.h"
#include "AICharacter_Enemy.generated.h"

//UENUM()
//enum class EState : uint8
//{
//	ES_Idle,
//	ES_Combat
//};

UCLASS()
class AIPROJECT_API AAICharacter_Enemy : public ACharacter, public IInterface_AIHelper
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter_Enemy();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFire_Broadcast);

	UPROPERTY(BlueprintAssignable)
	FFire_Broadcast Fire_Broadcast;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSubclassOf<class UAnimInstance> EnemyAnim;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class USkeletalMeshComponent* WeaponComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bIsWarning;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bIsFire;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class UHealthComponent* HealthComp;

	//class UUserDefinedEnum* StateEnum;

	FName EnumName;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class UAIPerceptionComponent* PerceptionComp;

	class UAISenseConfig_Sight* SenseConfig_Sight;

	class UAISenseConfig_Hearing* SenseConfig_Hearing;

	AActor* LastStimuliActor;

	FAIStimulus LastStimuliSensed;

	//bool bIsFireing;

	class UAnimMontage* FireMontage;

	TSubclassOf<class ABullet> BulletClass;

	class USoundWave* WeaponSound;

	bool bIsAutoFire = true;

	FTimerHandle FireHandle;

	int CurrentBulletCount = 0, MaxBulletCount = 6;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void UpdateSpeed(int Num);

	virtual int8 GetTeamID() override;
	virtual bool GetisDead() override;
	virtual void NotifyStateChanged(FName StateName) override;
	virtual APatrolPointManager* GetPatrol() override;
	virtual void SetAnimMode(bool IsWarning) override;
	virtual float GetAttackRange() override;

	UFUNCTION()
	void OnPerceptionChanged(const TArray<AActor*>& UpdatedActors);

	void PocessLastVision();

	void PocessLastHearing();

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	APatrolPointManager* GetCurrentPatrol();

	class APatrolPointManager* MyPatrol;

	UFUNCTION(BlueprintCallable, Category = "Anim")
	void SetAnimModeBlue(bool IsWarning);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	float GetAttackRangeBlue();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void EndFire();

	/*UFUNCTION(BlueprintCallable, Category = "Fire")
	bool GetIsFire();*/

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire();
};
