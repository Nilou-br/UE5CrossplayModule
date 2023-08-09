// Copyright © 2023 Melvin Brink

#include "Widgets/LobbyWidgetBase.h"
#include "Subsystems/Lobby/LobbySubsystem.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	ULobbySubsystem* LobbySubsystem = GetGameInstance()->GetSubsystem<ULobbySubsystem>();

	// Create
	LobbySubsystem->OnCreateLobbyCompleteDelegate.AddLambda([this](const ELobbyResultCode ResultCode, const FLobbyDetails& Details)
	{
		if(ResultCode == ELobbyResultCode::Success) OnCreateLobbySuccess(ResultCode, Details);
		else OnCreateLobbyFailure(ResultCode);
	});

	// Join
	LobbySubsystem->OnJoinLobbyCompleteDelegate.AddLambda([this](const ELobbyResultCode ResultCode, const FLobbyDetails& Details)
	{
		if(ResultCode == ELobbyResultCode::Success) OnJoinLobbySuccess(ResultCode, Details);
		else OnJoinLobbyFailure(ResultCode);
	});
}
