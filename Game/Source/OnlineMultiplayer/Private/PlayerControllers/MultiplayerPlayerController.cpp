// Copyright © 2023 Melvin Brink

#include "PlayerControllers/MultiplayerPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/User/Local/LocalUserSubsystem.h"


AMultiplayerPlayerController::AMultiplayerPlayerController()
{
	
}

void AMultiplayerPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (PlayerState != nullptr)
	{
		
	}
}
