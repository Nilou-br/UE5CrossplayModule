// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

struct FLobby;
struct FLobbyAttribute;
class UOnlineUser;
enum class ELeaveLobbyResultCode : uint8;
enum class EJoinLobbyResultCode : uint8;
enum class ECreateLobbyResultCode : uint8;



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
	void OnCreateLobbySuccess(const ECreateLobbyResultCode LobbyResultCode, const FLobby& Lobby);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnCreateLobbyFailure(const ECreateLobbyResultCode LobbyResultCode);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnJoinLobbySuccess(const EJoinLobbyResultCode LobbyResultCode, const FLobby& Lobby);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnJoinLobbyFailure(const EJoinLobbyResultCode LobbyResultCode);

	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnLeaveLobbySuccess(const ELeaveLobbyResultCode LobbyResultCode);
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnLeaveLobbyFailure(const ELeaveLobbyResultCode LobbyResultCode);

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby|Events")
	void OnLobbyAttributeChanged(const FLobbyAttribute& Attribute);

};
