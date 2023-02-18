// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGGAMEPRACTICEGameMode.h"
#include "RPGGAMEPRACTICECharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPGGAMEPRACTICEGameMode::ARPGGAMEPRACTICEGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
