// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API UAnimInstance_Enemy : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "StateName")
	void OnStateChanged(FName StateName);

	/*UFUNCTION(BlueprintImplementableEvent, Category = "Warning")
	void SetWarning(bool Warning);*/
};
