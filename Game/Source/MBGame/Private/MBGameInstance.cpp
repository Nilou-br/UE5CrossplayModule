// Copyright © 2023 Melvin Brink

#include "MBGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"



void UMBGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogMBGameInstance, Log, TEXT("Initializing MBGameInstance"));

	if (LoadingScreenWidgetClass) LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);
	if (LobbyOverlayWidgetClass) LobbyOverlayWidget = CreateWidget<UUserWidget>(this, LobbyOverlayWidgetClass);
	
	// Viewport will be ready next frame, calling this earlier will cause a crash.
	// Easier than putting it in a separate GameMode's BeginPlay and adding a bunch of checks for login-state etc.
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		ShowLoadingScreen();
		
		UE_LOG(LogMBGameInstance, Log, TEXT("Waiting until module is ready..."));
		WaitUntilReady([this](bool bWasSuccessful)
		{
			if(bWasSuccessful) HideLoadingScreen();
		});
	});
}

void UMBGameInstance::Shutdown()
{
	UE_LOG(LogMBGameInstance, Log, TEXT("Shutdown MBGameInstance"));

	//
	
	Super::Shutdown();
}


// --------------------------------


void UMBGameInstance::ShowLoadingScreen()
{
	if(!LoadingScreenWidget)
	{
		UE_LOG(LogMBGameInstance, Log, TEXT("No valid LoadingScreen Widget."));
	}

	LoadingScreenWidget->AddToViewport();
}

void UMBGameInstance::HideLoadingScreen()
{
	if(!LoadingScreenWidget)
	{
		UE_LOG(LogMBGameInstance, Log, TEXT("No valid LoadingScreen Widget."));
	}

	LoadingScreenWidget->RemoveFromParent();
}


// --------------------------------


UUserWidget* UMBGameInstance::GetLobbyOverlayWidget() const
{
	if (!LobbyOverlayWidget) UE_LOG(LogMBGameInstance, Log, TEXT("No valid Lobby-Overlay Widget."));
	return LobbyOverlayWidget;
}
