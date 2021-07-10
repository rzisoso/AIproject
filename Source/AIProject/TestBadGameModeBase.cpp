// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBadGameModeBase.h"
#include "CrossHUD.h"
#include <UObject/ConstructorHelpers.h>
#include "FirstPersonCharacter.h"

ATestBadGameModeBase::ATestBadGameModeBase()
{
	HUDClass = ACrossHUD::StaticClass();
	static ConstructorHelpers::FClassFinder<AFirstPersonCharacter> TempPawnClass(TEXT("/Game/Blueprint/BP_FirstPersonCharacter"));
	if (TempPawnClass.Succeeded())
	{
		DefaultPawnClass = TempPawnClass.Class;
	}
	
}