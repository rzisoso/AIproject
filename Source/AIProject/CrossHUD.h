// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrossHUD.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API ACrossHUD : public AHUD
{
	GENERATED_BODY()

public:

	ACrossHUD();
	
protected:

	virtual void BeginPlay() override;

	class UTexture2D* CrossTexture;

public:

	virtual void DrawHUD() override;
};
