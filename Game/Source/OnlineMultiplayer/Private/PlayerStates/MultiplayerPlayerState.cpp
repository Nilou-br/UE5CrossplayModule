// Copyright © 2023 Melvin Brink

#include "PlayerStates/MultiplayerPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/Lobby/LobbySubsystem.h"


AMultiplayerPlayerState::AMultiplayerPlayerState()
{
	bReplicates = true;
	Username = "Unknown";
}

void AMultiplayerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ProductUserID);
}

void AMultiplayerPlayerState::OnRep_ProductUserID()
{
	ULobbySubsystem* LobbySubsystem = GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	if(UOnlineUser** FoundUser = LobbySubsystem->GetLobby().GetMember(ProductUserID); FoundUser && *FoundUser)
	{
		Username = (*FoundUser)->GetUsername();
	}
}
