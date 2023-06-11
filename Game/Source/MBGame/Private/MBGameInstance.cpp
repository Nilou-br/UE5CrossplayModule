// Copyright © 2023 Melvin Brink

#include "MBGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Platforms/EOS/Subsystems/ConnectSubsystem.h"
#include "Platforms/EOS/Subsystems/FriendsSubsystem.h"



void UMBGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogMBGameInstance, Log, TEXT("Initializing MBGameInstance"));

	if (LoadingScreenWidgetClass) LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);
	if (PartyOverlayWidgetClass) PartyOverlayWidget = CreateWidget<UUserWidget>(this, PartyOverlayWidgetClass);

	// Viewport will be ready next frame.
	// Easier than putting it in a separate GameMode's BeginPlay and adding a bunch of checks for login-state etc.
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		StartupGame();
	});
}

void UMBGameInstance::Shutdown()
{
	UE_LOG(LogMBGameInstance, Log, TEXT("Shutdown MBGameInstance"));

	//
	
	Super::Shutdown();
}


// --------------------------------


/**
 * Call this function once at the start of the game from the UMBGameInstance::Init().
 *
 * Will load all necessary things and opens the level when complete.
 *
 * Currently does a login to connect interface. // TODO: add auth interface as well.
 *
 * Update this function when more is needed.
 */
void UMBGameInstance::StartupGame()
{
	PendingOperations = 1;
	ShowLoadingScreen();

	// Login to connect-interface.
	UConnectSubsystem* ConnectSubsystem = GetSubsystem<UConnectSubsystem>();
	ConnectSubsystem->OnConnectLoginCompleteDelegate.Clear();
	ConnectSubsystem->OnConnectLoginCompleteDelegate.BindDynamic(this, &UMBGameInstance::OnConnectLoginComplete);
	ConnectSubsystem->Login();
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


void UMBGameInstance::OnConnectLoginComplete(const bool bSuccess, UEosUser* LocalUser)
{
	HideLoadingScreen();
	
	UConnectSubsystem* ConnectSubsystem = GetSubsystem<UConnectSubsystem>();
	if(ConnectSubsystem) ConnectSubsystem->OnConnectLoginCompleteDelegate.Clear();
	
	if(bSuccess)
	{
		
	}else
	{
		UE_LOG(LogMBGameInstance, Log, TEXT("Login to EOS-Connect-Interface failed."));
		// TODO: Error ResultCode.
	}
}


// --------------------------------


UUserWidget* UMBGameInstance::GetPartyOverlayWidget() const
{
	if (!PartyOverlayWidget) UE_LOG(LogMBGameInstance, Log, TEXT("No valid PartyOverlay Widget."));
	return PartyOverlayWidget;
}
