// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossHUD.h"
#include <Engine/Texture2D.h>
#include <UObject/ConstructorHelpers.h>
#include <Engine/Canvas.h>

ACrossHUD::ACrossHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> TempTexture(TEXT("/Game/Texture/FirstPersonCrosshair"));
	if (TempTexture.Succeeded())
	{
		CrossTexture = TempTexture.Object;
	}
}

void ACrossHUD::BeginPlay()
{
	Super::BeginPlay();


}

void ACrossHUD::DrawHUD()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReceiveDrawHUD"));
	if (CrossTexture)
	{
		float ScreenX = Canvas->SizeX / 2 - 50.f, ScreenY = Canvas->SizeY / 2 + 50.f;
		DrawTexture(CrossTexture, ScreenX, ScreenY, 16.f, 16.f, 0.f, 0.f, 1.f, 1.f);
	}
}

