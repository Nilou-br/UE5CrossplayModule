// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

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
};
