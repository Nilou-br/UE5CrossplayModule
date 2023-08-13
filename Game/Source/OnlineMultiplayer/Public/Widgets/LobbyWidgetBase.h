// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

class UOnlineUser;



/**
 * Base widget class that listens to lobby updates.
 * Widgets deriving from this class can listen to the events declared here.
 */
UCLASS()
class ONLINEMULTIPLAYER_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnCreateLobbySuccess(const ELobbyResultCode LobbyResultCode, const FLobbyDetails& LobbyDetails);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnCreateLobbyFailure(const ELobbyResultCode LobbyResultCode);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnJoinLobbySuccess(const ELobbyResultCode LobbyResultCode, const FLobbyDetails& LobbyDetails);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnJoinLobbyFailure(const ELobbyResultCode LobbyResultCode);

	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnUserJoined(const UOnlineUser* OnlineUser);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnUserLeft(const FString& ProductUserID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnUserDisconnected(const FString& ProductUserID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnUserKicked(const FString& ProductUserID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnUserPromoted(const FString& ProductUserID);
};
