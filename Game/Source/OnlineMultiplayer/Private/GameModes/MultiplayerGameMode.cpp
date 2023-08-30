// Copyright © 2023 Melvin Brink

#include "GameModes/MultiplayerGameMode.h"

#include "Subsystems/Session/SessionSubsystem.h"


void AMultiplayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer->IsLocalController() && HasAuthority())
	{
		const USessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<USessionSubsystem>();
		SessionSubsystem->OnServerCreatedDelegate.Broadcast();
	}
	else
	{
		// This must be a client joining the server, do client setup here.
	}
}
