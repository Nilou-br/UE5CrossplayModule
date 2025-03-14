// Copyright © 2023 Melvin Brink

#include "Widgets/LobbyWidgetBase.h"
#include "Subsystems/Lobby/LobbySubsystem.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	if(bEventsBound) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Binding events to the LobbyWidgetBase..."))
	ULobbySubsystem* LobbySubsystem = GetGameInstance()->GetSubsystem<ULobbySubsystem>();
	
	// Create
	OnCreateLobbyCompleteHandle = LobbySubsystem->OnCreateLobbyCompleteDelegate.AddLambda([this](const ECreateLobbyResultCode ResultCode, const FLobby& Lobby)
	{
		if(ResultCode == ECreateLobbyResultCode::Success) OnCreateLobbySuccess(ResultCode, Lobby);
		else OnCreateLobbyFailure(ResultCode);
	});

	// Join
	OnJoinLobbyCompleteHandle = LobbySubsystem->OnJoinLobbyCompleteDelegate.AddLambda([this](const EJoinLobbyResultCode ResultCode, const FLobby& Lobby)
	{
		if(ResultCode == EJoinLobbyResultCode::Success) OnJoinLobbySuccess(ResultCode, Lobby);
		else OnJoinLobbyFailure(ResultCode);
	});

	// Leave
	OnLeaveLobbyCompleteHandle = LobbySubsystem->OnLeaveLobbyCompleteDelegate.AddLambda([this](const ELeaveLobbyResultCode ResultCode)
	{
		if(ResultCode == ELeaveLobbyResultCode::Success || ResultCode == ELeaveLobbyResultCode::NotInLobby) OnLeaveLobbySuccess(ResultCode);
		else OnLeaveLobbyFailure(ResultCode);
	});

	
	// Lobby Started
	OnLobbyStartedHandle = LobbySubsystem->OnLobbyStartedDelegate.AddLambda([this](const FString& ServerAddress){ OnLobbyStarted(ServerAddress); });

	// Lobby Stopped
	OnLobbyStoppedHandle = LobbySubsystem->OnLobbyStoppedDelegate.AddLambda([this](){ OnLobbyStopped(); });

	
	// User Joined
	OnUserJoinedHandle = LobbySubsystem->OnLobbyUserJoinedDelegate.AddLambda([this](const UOnlineUser* OnlineUser){ OnUserJoined(OnlineUser); });

	// User Left
	OnUserLeftHandle = LobbySubsystem->OnLobbyUserLeftDelegate.AddLambda([this](const FString& ProductUserID){ OnUserLeft(ProductUserID); });

	// User Disconnected
	OnUserDisconnectedHandle = LobbySubsystem->OnLobbyUserDisconnectedDelegate.AddLambda([this](const FString& ProductUserID){ OnUserDisconnected(ProductUserID); });

	// User Kicked
	OnUserKickedHandle = LobbySubsystem->OnLobbyUserKickedDelegate.AddLambda([this](const FString& ProductUserID){ OnUserKicked(ProductUserID); });

	// User Promoted
	OnUserPromotedHandle = LobbySubsystem->OnLobbyUserPromotedDelegate.AddLambda([this](const FString& ProductUserID){ OnUserPromoted(ProductUserID); });

	
	// Attribute changed
	OnLobbyAttributeChangedHandle = LobbySubsystem->OnLobbyAttributeChanged.AddLambda([this](const FLobbyAttribute& Attribute){ OnLobbyAttributeChanged(Attribute); });

	
	// To check whether these events have been bound already, because this method gets called when starting a server.
	bEventsBound = true;
}

void ULobbyWidgetBase::BeginDestroy()
{
	if(GetGameInstance())
	{
		UE_LOG(LogTemp, Warning, TEXT("Unbinding events from the LobbyWidgetBase..."))
		ULobbySubsystem* LobbySubsystem = GetGameInstance()->GetSubsystem<ULobbySubsystem>();
		
		LobbySubsystem->OnCreateLobbyCompleteDelegate.Remove(OnCreateLobbyCompleteHandle);
		LobbySubsystem->OnJoinLobbyCompleteDelegate.Remove(OnJoinLobbyCompleteHandle);
		LobbySubsystem->OnLeaveLobbyCompleteDelegate.Remove(OnLeaveLobbyCompleteHandle);
	
		LobbySubsystem->OnLobbyStartedDelegate.Remove(OnLobbyStartedHandle);
		LobbySubsystem->OnLobbyStoppedDelegate.Remove(OnLobbyStoppedHandle);
	
		LobbySubsystem->OnLobbyUserJoinedDelegate.Remove(OnUserJoinedHandle);
		LobbySubsystem->OnLobbyUserLeftDelegate.Remove(OnUserLeftHandle);
		LobbySubsystem->OnLobbyUserDisconnectedDelegate.Remove(OnUserDisconnectedHandle);
		LobbySubsystem->OnLobbyUserKickedDelegate.Remove(OnUserKickedHandle);
		LobbySubsystem->OnLobbyUserPromotedDelegate.Remove(OnUserPromotedHandle);

		LobbySubsystem->OnLobbyAttributeChanged.Remove(OnLobbyAttributeChangedHandle);
	}
	
	Super::BeginDestroy();
}
