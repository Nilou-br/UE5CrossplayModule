// Copyright © 2023 Melvin Brink

#include "FunctionLibraries/LobbyFunctionLibrary.h"
#include "Subsystems/Lobby/LobbySubsystem.h"



/**
 * Create a lobby which can be joined by any platform.
 */
void ULobbyFunctionLibrary::CreateLobby(UObject* WorldContextObject, const uint8 MaxMembers)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->CreateLobby(MaxMembers);
}

/**
 * Join a lobby using the given Lobby ID
 */
void ULobbyFunctionLibrary::JoinLobbyByLobbyID(UObject* WorldContextObject, const FString& LobbyID)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->JoinLobbyByID(LobbyID);
}

/**
 * Join a lobby using the given User ID
 */
void ULobbyFunctionLibrary::JoinLobbyByUserID(UObject* WorldContextObject, const FString& UserID)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->JoinLobbyByUserID(UserID);
}

/**
 * Set/update a single attribute on the lobby.
 * You should use 'SetLobbyAttributes' if you want to set multiple at once.
 *
 * Other members will be notified using the OnLobbyAttributeChanged delegate.
 */
void ULobbyFunctionLibrary::SetLobbyAttribute(UObject* WorldContextObject, const FLobbyAttribute& Attribute)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->SetAttributes(TArray<FLobbyAttribute>{Attribute}, nullptr); // todo callback
}

/**
 * Set/update multiple attributes on the lobby.
 *
 * Other members will be notified using the OnLobbyAttributeChanged delegate.
 */
void ULobbyFunctionLibrary::SetLobbyAttributes(UObject* WorldContextObject, const TArray<FLobbyAttribute> Attributes)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->SetAttributes(Attributes, nullptr); // todo callback
}