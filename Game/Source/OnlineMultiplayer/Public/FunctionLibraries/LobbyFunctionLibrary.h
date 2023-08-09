// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LobbyFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEMULTIPLAYER_API ULobbyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Lobby|Actions", meta = (WorldContext = "WorldContextObject"))
	static void CreateLobby(UObject* WorldContextObject, const uint8 MaxMembers);
	
	UFUNCTION(BlueprintCallable, Category = "Lobby|Actions", meta = (WorldContext = "WorldContextObject"))
	static void JoinLobbyByLobbyID(UObject* WorldContextObject, const FString &LobbyID);
	
	UFUNCTION(BlueprintCallable, Category = "Lobby|Actions", meta = (WorldContext = "WorldContextObject"))
	static void JoinLobbyByUserID(UObject* WorldContextObject, const FString &UserID);
};
