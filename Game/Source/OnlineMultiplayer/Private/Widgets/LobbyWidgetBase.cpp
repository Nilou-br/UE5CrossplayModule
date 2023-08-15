// Copyright © 2023 Melvin Brink

#include "Widgets/LobbyWidgetBase.h"
#include "Subsystems/Lobby/LobbySubsystem.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	ULobbySubsystem* LobbySubsystem = GetGameInstance()->GetSubsystem<ULobbySubsystem>();

	// Create
	LobbySubsystem->OnCreateLobbyCompleteDelegate.AddLambda([this](const ECreateLobbyResultCode ResultCode, const FLobbyDetails& Details)
	{
		if(ResultCode == ECreateLobbyResultCode::Success) OnCreateLobbySuccess(ResultCode, Details);
		else OnCreateLobbyFailure(ResultCode);
	});

	// Join
	LobbySubsystem->OnJoinLobbyCompleteDelegate.AddLambda([this](const EJoinLobbyResultCode ResultCode, const FLobbyDetails& Details)
	{
		if(ResultCode == EJoinLobbyResultCode::Success) OnJoinLobbySuccess(ResultCode, Details);
		else OnJoinLobbyFailure(ResultCode);
	});

	// Leave
	LobbySubsystem->OnLeaveLobbyCompleteDelegate.AddLambda([this](const ELeaveLobbyResultCode ResultCode)
	{
		if(ResultCode == ELeaveLobbyResultCode::Success || ResultCode == ELeaveLobbyResultCode::NotInLobby) OnLeaveLobbySuccess(ResultCode);
		else OnLeaveLobbyFailure(ResultCode);
	});

	// User Joined
	LobbySubsystem->OnLobbyUserJoinedDelegate.AddLambda([this](const UOnlineUser* OnlineUser){ OnUserJoined(OnlineUser); });

	// User Left
	LobbySubsystem->OnLobbyUserLeftDelegate.AddLambda([this](const FString& ProductUserID){ OnUserLeft(ProductUserID); });

	// User Disconnected
	LobbySubsystem->OnLobbyUserDisconnectedDelegate.AddLambda([this](const FString& ProductUserID){ OnUserDisconnected(ProductUserID); });

	// User Kicked
	LobbySubsystem->OnLobbyUserKickedDelegate.AddLambda([this](const FString& ProductUserID){ OnUserKicked(ProductUserID); });

	// User Promoted
	LobbySubsystem->OnLobbyUserPromotedDelegate.AddLambda([this](const FString& ProductUserID){ OnUserPromoted(ProductUserID); });
}
