﻿// Copyright © 2023 Melvin Brink

#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4265)
#include "steam_api.h"
#include "steam_api_common.h"
#include "isteamuser.h"
#pragma warning(pop)

#include "CoreMinimal.h"
#include <string>
#include "SteamUserSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSteamUserSubsystem, Log, All);
inline DEFINE_LOG_CATEGORY(LogSteamUserSubsystem);

#define CHECK_STEAM if(!SteamAPI_Init()){ UE_LOG(LogSteamUserSubsystem, Error, TEXT("Steam SDK is not initialized.")); return; }

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEncryptedAppTicketReady, TArray<uint8>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionTicketReady, TArray<uint8>);



/**
 * Subsystem for managing the Steam user.
 */
UCLASS(NotBlueprintable)
class ONLINEMULTIPLAYER_API USteamUserSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	// Tickets
	void RequestEncryptedAppTicket();
	FOnEncryptedAppTicketReady OnEncryptedAppTicketReady;
	void RequestSessionTicket();
	FOnSessionTicketReady OnSessionTicketReady;
	
private:
	// Ticket callbacks
	void OnEncryptedAppTicketResponse( EncryptedAppTicketResponse_t *pEncryptedAppTicketResponse, bool bIOFailure );
	CCallResult<USteamUserSubsystem, EncryptedAppTicketResponse_t> m_EncryptedAppTicketResponseCallResult;
	STEAM_CALLBACK(USteamUserSubsystem, OnSessionTicketResponse, GetAuthSessionTicketResponse_t);

	// Session ticket data
	TArray<uint8> SessionTicket;
	bool bWaitingForSessionTicket = false;
	bool bSessionTicketReady = false;
	
	SteamNetworkingIdentity Identity;

public:
	FORCEINLINE void SetRichPresence(const char *Key, const char *Value) { SteamFriends()->SetRichPresence(Key, Value); }
	FORCEINLINE CSteamID GetSteamID() const { return SteamUser()->GetSteamID(); }
};
