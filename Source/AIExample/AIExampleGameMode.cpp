// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIExampleGameMode.h"
#include "AIExampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAIExampleGameMode::AAIExampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
