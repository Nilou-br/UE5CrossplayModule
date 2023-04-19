// Copyright © 2023 Melvin Brink

#include "Multiplayer/OnlineSubsystem/OnlineManagerProxySubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Multiplayer/OnlineSubsystem/PartyManager.h"
#include "Multiplayer/OnlineSubsystem/SessionManager.h"
#include "OnlineSubsystemEOSPlus/Private/OnlineSessionEOSPlus.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOnlineManagerProxySubsystem, Log, All);
DEFINE_LOG_CATEGORY(LogOnlineManagerProxySubsystem);

// #define LOG_PARTY_NULL UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("UPartyManagerProxySubsystem: PartyManagerProxy is nullptr"))
// #define CHECK_SUBSYSTEM_NULL if (!OnlineSubsystem) { UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("Subsystem is null")); return; }



void UOnlineManagerProxySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Get the OnlineSubsystem pointer for initializing OSSPlus.
	const IOnlineSubsystemPtr OnlineSubsystemEOSPlus = MakeShareable(IOnlineSubsystem::Get("EOSPlus"));
	if(!OnlineSubsystemEOSPlus.IsValid()) return;
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("OSS EOSPlus exists"));

	// Set subsystems.
	OSSPlus = StaticCastSharedPtr<FOnlineSubsystemEOSPlus>(OnlineSubsystemEOSPlus);
	if(!OSSPlus.IsValid()) return;
	OSSBase = OSSPlus->BaseOSS;
	if (OSSBase) UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("OSSBase: %s"), *OSSBase->GetSubsystemName().ToString());

	// Set the session interface and set the UseEOSSession to false initially.
	OSSPlusSessionInterface = StaticCastSharedPtr<FOnlineSessionEOSPlus>(OSSPlus->GetSessionInterface());
	OSSPlusSessionInterface->SetUseEOSSessions(false);
			
	// Set the UniquePlayerId to the BaseOS Net-ID at startup (Steam).
	if(!OSSBase) return;
	const IOnlineIdentityPtr Identity = OSSBase->GetIdentityInterface();
	if(Identity.IsValid()) UniquePlayerId = Identity->GetUniquePlayerId(0);
	if(UniquePlayerId.IsValid()) UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("UniquePlayerId: %s"), *UniquePlayerId->ToString());
	if(!UniquePlayerId.IsValid()) UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("UniquePlayerId is invalid"));

}



/*
 Login/Logout.
 */

void UOnlineManagerProxySubsystem::Login()
{
	OSSPlusSessionInterface->SetUseEOSSessions(true); // Set the UseEOSSession to true, Otherwise EOSPlus will use the BaseOSS-subsystem (Steam).

	if(IOnlineIdentityPtr Identity = OSSPlus->GetIdentityInterface())
	{
		FOnlineAccountCredentials Credentials;
		Credentials.Type = FString("steam");
		Identity->OnLoginCompleteDelegates->AddUObject(this, &ThisClass::OnLoginComplete);
		Identity->Login(0, Credentials);
	}
}

void UOnlineManagerProxySubsystem::Logout()
{
	
	if(IOnlineIdentityPtr Identity = OSSPlus->GetIdentityInterface())
	{
		Identity->OnLogoutCompleteDelegates->AddUObject(this, &ThisClass::OnLogoutComplete);
		Identity->Logout(0);
	}
}



/*
 Login/Logout Complete.
 */

void UOnlineManagerProxySubsystem::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("Login complete: %d"), bWasSuccessful);
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("Error: %s"), *Error);
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("User ID: %s"), *UserId.ToString());
	
	// Clear delegate.
	const IOnlineIdentityPtr Identity = OSSPlus->GetIdentityInterface();
	if(!Identity.IsValid()) return;
	Identity->ClearOnLoginCompleteDelegates(0, this);
	
	// Set the UniquePlayerId based on the login result.
	if(bWasSuccessful && UserId.IsValid())
	{
		// Use the Epic Online Services Unique Net ID. This is a combination of the Steam ID and the Epic Online Services Account ID.
		UniquePlayerId = UserId.AsShared();
		UE_LOG(LogTemp, Warning, TEXT("Logged-in EOSPlus: %s"), *UniquePlayerId->ToString());
		OSSPlusSessionInterface->SetUseEOSSessions(true); // Set it to true to use the EOS session system.
	}
	else
	{
		// Set the UniquePlayerId to the Steam ID.
		if(!OSSBase) return;
		const IOnlineIdentityPtr BaseIdentity = OSSBase->GetIdentityInterface();
		if(BaseIdentity.IsValid()) UniquePlayerId = BaseIdentity->GetUniquePlayerId(0);
		OSSPlusSessionInterface->SetUseEOSSessions(false); // Set it to false to use the Steam session system.
	}
	
	if(UniquePlayerId.IsValid()) UE_LOG(LogTemp, Warning, TEXT("UniquePlayerId: %s"), *UniquePlayerId->ToString());
}

void UOnlineManagerProxySubsystem::OnLogoutComplete(int32 LocalUserNum, bool bWasSuccessful)
{
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("Logout complete: %d"), bWasSuccessful);

	// Clear delegate.
	const IOnlineIdentityPtr Identity = OSSPlus->GetIdentityInterface();
	if(!Identity.IsValid()) return;
	Identity->ClearOnLoginCompleteDelegates(0, this);

	if(bWasSuccessful)
	{
		// Set the UniquePlayerId to the Steam ID.
		if(!OSSBase) return;
		const IOnlineIdentityPtr SteamIdentity = OSSBase->GetIdentityInterface();
		if(SteamIdentity.IsValid()) UniquePlayerId = SteamIdentity->GetUniquePlayerId(0);
		OSSPlusSessionInterface->SetUseEOSSessions(false); // Set it to false to use the Steam session system.	
	}
}



/*
 Session.
 */

void UOnlineManagerProxySubsystem::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating session using: %s"), *OSSPlus->GetSubsystemName().ToString());
	if(!OSSPlusSessionInterface.IsValid()) return;
	
	if(OSSPlusSessionInterface->GetNamedSession(NAME_GameSession))
	{
		OSSPlusSessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionComplete);
		OSSPlusSessionInterface->DestroySession(NAME_GameSession);
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.Set(SEARCH_KEYWORDS, FString("MBGameLobby"), EOnlineDataAdvertisementType::ViaOnlineService); // Makes it visible in Dev-Portal.
	
	OSSPlusSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);
	OSSPlusSessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
}

void UOnlineManagerProxySubsystem::JoinSession()
{
	
}



/*
 Session Complete
 */

void UOnlineManagerProxySubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("OnCreateSessionComplete : %d"), bWasSuccessful);

	const IOnlineSessionPtr SessionInterface = OSSPlus->GetSessionInterface();
	if(SessionInterface.IsValid()) SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
}

void UOnlineManagerProxySubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("OnDestroySessionComplete : %d"), bWasSuccessful);
	const IOnlineSessionPtr SessionInterface = OSSPlus->GetSessionInterface();
	if(!SessionInterface.IsValid()) return;
	SessionInterface->ClearOnDestroySessionCompleteDelegates(this);
	CreateSession();
}


/*
 Party.
 */



/*
 Party Complete.
 */