// Copyright Epic Games, Inc. All Rights Reserved.

#include "DES311_ShooterGameMode.h"
#include "DES311_ShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADES311_ShooterGameMode::ADES311_ShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
