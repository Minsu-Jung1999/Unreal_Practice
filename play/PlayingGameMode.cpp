// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayingGameMode.h"
#include "PlayingCharacter.h"
#include "UObject/ConstructorHelpers.h"

APlayingGameMode::APlayingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
