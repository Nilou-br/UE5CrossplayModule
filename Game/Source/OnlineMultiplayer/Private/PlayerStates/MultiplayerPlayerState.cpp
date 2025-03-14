// Copyright © 2023 Melvin Brink

#include "PlayerStates/MultiplayerPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/Lobby/LobbySubsystem.h"
#include "Subsystems/User/Local/LocalUserSubsystem.h"



AMultiplayerPlayerState::AMultiplayerPlayerState()
{
	bReplicates = true;
}

void AMultiplayerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ProductUserID);
}

void AMultiplayerPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Ensure we are on the client and then run the server RPC to set the ProductUserID.
	if (GetNetMode() == NM_Client)
	{
		const ULocalUserSubsystem* LocalUserSubsystem = GetGameInstance()->GetSubsystem<ULocalUserSubsystem>();
		ServerSetProductUserID(LocalUserSubsystem->GetLocalUser()->GetProductUserID());
	}
}

void AMultiplayerPlayerState::ServerSetProductUserID_Implementation(const FString& InProductUserID)
{
	ProductUserID = InProductUserID;
	OnPlayerProductIDSetDelegate.Broadcast(InProductUserID);

	ULobbySubsystem* LobbySubsystem = GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	UOnlineUser** FoundUserPtr = LobbySubsystem->GetLobby().GetMember(ProductUserID);
	if (FoundUserPtr == nullptr || *FoundUserPtr == nullptr)
	{
		UE_LOG(LogMPPlayerState, Error, TEXT("User not found in 'AMultiplayerPlayerState::OnRep_ProductUserID' for ProductUserID: '%s'"), *ProductUserID);
		SetPlayerName("Unknown");
		return;
	}
	
	const UOnlineUser* FoundUser = *FoundUserPtr;
	SetPlayerName(FoundUser->GetUsername());
}
