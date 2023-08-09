// Copyright © 2023 Melvin Brink

#include "FunctionLibraries/LobbyFunctionLibrary.h"
#include "Subsystems/Lobby/LobbySubsystem.h"



/*
 * Create a lobby which can be joined by any platform.
 */
void ULobbyFunctionLibrary::CreateLobby(UObject* WorldContextObject, const uint8 MaxMembers)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->CreateLobby(MaxMembers);
}

/*
 * Join a lobby using the given Lobby ID
 */
void ULobbyFunctionLibrary::JoinLobbyByLobbyID(UObject* WorldContextObject, const FString& LobbyID)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->JoinLobbyByID(LobbyID);
}

/*
 * Join a lobby using the given User ID
 */
void ULobbyFunctionLibrary::JoinLobbyByUserID(UObject* WorldContextObject, const FString& UserID)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;
	
	ULobbySubsystem* LobbySubsystem = World->GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	LobbySubsystem->JoinLobbyByUserID(UserID);
}
