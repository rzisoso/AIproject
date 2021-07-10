// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface_AIHelper.h"
#include <Components/TimelineComponent.h>
#include "FirstPersonCharacter.generated.h"

UCLASS()
class AIPROJECT_API AFirstPersonCharacter : public ACharacter, public IInterface_AIHelper
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class USkeletalMeshComponent* FP_Gun;

	bool bIsFire;

	class UAnimMontage* FireMontage;

	TSubclassOf<class ABullet> BulletClass;

	class USoundWave* WeaponSound;

	UPROPERTY(VisibleAnywhere, Category = "Comp")
	class UHealthComponent* HealthComp;

	FName EnumName;

	class UCurveFloat* PostCurve;

	FTimeline PostTimeline;

	// InterpFunc;

	float PostVal = 0.f;

	bool bIsDead;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Jump();

	void MoveForward(float Val);

	void MoveRight(float Val);

	void Turn(float Val);

	void LookUp(float Val);

	void StartFire();

	void EndFire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool GetIsFire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire();

	virtual int8 GetTeamID() override;
	virtual bool GetisDead() override;
	virtual void NotifyStateChanged(FName StateName) override;

	UFUNCTION(BlueprintCallable, Category = "Dead")
	bool GetIsDeadBlue();

	void Dead();

	UFUNCTION()
	void PostFunc(float Val);
};
