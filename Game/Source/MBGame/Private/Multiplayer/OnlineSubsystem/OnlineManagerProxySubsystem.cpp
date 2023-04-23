// Copyright © 2023 Melvin Brink

#include "Multiplayer/OnlineSubsystem/OnlineManagerProxySubsystem.h"

#include "EOSSettings.h"
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
	
	OnlineSubsystem = IOnlineSubsystem::Get("EOSPlus");
}



/*
 Login/Logout.
 */

void UOnlineManagerProxySubsystem::Login()
{
	// OSSPlusSessionInterface->SetUseEOSSessions(true); // Set the UseEOSSession to true, Otherwise EOSPlus will use the BaseOSS-subsystem (Steam).
	if(const IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
	{
		UEOSSettings::SetUseEAS(true);
		FOnlineAccountCredentials Credentials;
		Credentials.Type = FString("steam");
		Identity->OnLoginCompleteDelegates->AddUObject(this, &ThisClass::OnLoginComplete);
		Identity->Login(0, Credentials);
	}
}

void UOnlineManagerProxySubsystem::Logout()
{
	
	if(const IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
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
	const IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
	if(!Identity.IsValid()) return;
	Identity->ClearOnLoginCompleteDelegates(0, this);

	
}

void UOnlineManagerProxySubsystem::OnLogoutComplete(int32 LocalUserNum, bool bWasSuccessful)
{
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("Logout complete: %d"), bWasSuccessful);
	UEOSSettings::SetUseEAS(false);
	
	// Clear delegate.
	const IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
	if(!Identity.IsValid()) return;
	Identity->ClearOnLoginCompleteDelegates(0, this);
}



/*
 Session.
 */

void UOnlineManagerProxySubsystem::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating session using: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
	const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	
	if(!SessionInterface.IsValid()) return;
	
	if(SessionInterface->GetNamedSession(NAME_GameSession))
	{
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionComplete);
		SessionInterface->DestroySession(NAME_GameSession);
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
	
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);
	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
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

	const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if(SessionInterface.IsValid()) SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
}

void UOnlineManagerProxySubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnlineManagerProxySubsystem, Warning, TEXT("OnDestroySessionComplete : %d"), bWasSuccessful);
	const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
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