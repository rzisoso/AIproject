// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIProjectGameMode.h"
#include "AIProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAIProjectGameMode::AAIProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
