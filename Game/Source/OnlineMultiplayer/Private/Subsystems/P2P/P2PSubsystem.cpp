// Copyright © 2023 Melvin Brink


#include "Subsystems/P2P/P2PSubsystem.h"
#include "EOSManager.h"
#include "eos_p2p.h"
#include "eos_sdk.h"
#include "Helpers.h"
#include "Subsystems/User/Local/LocalUserSubsystem.h"


UP2PSubsystem::UP2PSubsystem() : EosManager(&FEosManager::Get())
{
}

void UP2PSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LocalUserSubsystem = Collection.InitializeDependency<ULocalUserSubsystem>();

	const EOS_HPlatform PlatformHandle = EosManager->GetPlatformHandle();
	P2PHandle = EOS_Platform_GetP2PInterface(PlatformHandle);

	EOS_P2P_AddNotifyPeerConnectionRequestOptions DirectMessageRequestOptions;
	DirectMessageRequestOptions.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
	DirectMessageRequestOptions.LocalUserId = EosProductIDFromString(LocalUserSubsystem->GetLocalUser()->GetProductUserID());
	DirectMessageRequestNotification = EOS_P2P_AddNotifyPeerConnectionRequest(P2PHandle, &DirectMessageRequestOptions, this, &ThisClass::OnConnectionRequest);

	EOS_P2P_AddNotifyPeerConnectionRequestOptions LobbyMessageRequestOptions;
	LobbyMessageRequestOptions.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
	LobbyMessageRequestOptions.LocalUserId = EosProductIDFromString(LocalUserSubsystem->GetLocalUser()->GetProductUserID());
	constexpr EOS_P2P_SocketId LobbyMessageSocketID = { EOS_P2P_SOCKETID_API_LATEST, "LobbyID" };
	LobbyMessageRequestOptions.SocketId = &LobbyMessageSocketID;
	LobbyMessageRequestNotification = EOS_P2P_AddNotifyPeerConnectionRequest(P2PHandle, &LobbyMessageRequestOptions, this, &ThisClass::OnConnectionRequest);
}

void UP2PSubsystem::Deinitialize()
{
	EOS_P2P_RemoveNotifyPeerConnectionRequest(P2PHandle, DirectMessageRequestNotification);
	EOS_P2P_RemoveNotifyPeerConnectionRequest(P2PHandle, LobbyMessageRequestNotification);
	
	Super::Deinitialize();
}

void UP2PSubsystem::OnConnectionRequest(const EOS_P2P_OnIncomingConnectionRequestInfo* Data)
{
	
}
