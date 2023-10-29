// Copyright Epic Games, Inc. All Rights Reserved.

#include "NorthPlusJamGameMode.h"
#include "NorthPlusJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANorthPlusJamGameMode::ANorthPlusJamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
