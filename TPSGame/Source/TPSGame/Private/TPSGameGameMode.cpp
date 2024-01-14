// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/TPSPlayerController.h"

ATPSGameGameMode::ATPSGameGameMode()
{

		DefaultPawnClass = nullptr;
	   PlayerControllerClass=ATPSPlayerController::StaticClass();
	
}
