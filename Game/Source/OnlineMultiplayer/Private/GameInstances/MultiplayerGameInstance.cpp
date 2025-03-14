// Copyright © 2023 Melvin Brink

#include "GameInstances/MultiplayerGameInstance.h"
#include "Subsystems/Connect/ConnectSubsystem.h"



/**
 * Waits until all async operations that happen at the start of the game have finished. For example: logging into the connect/auth interfaces.
 *
 * Call this from the game-instance derived from this base class.
 *
 * @param Callback the callback to run after the subsystems are initialized.
 */
void UMultiplayerGameInstance::WaitUntilReady(TFunction<void(bool bWasSuccessful)>const& Callback)
{
	if(!Callback)
	{
		UE_LOG(LogMultiplayerGameInstance, Error, TEXT("Invalid callback passed into UMultiplayerGameInstance::WaitUntilReady."))
		return;
	}
	
	if(InitializeCount == 0)
	{
		Callback(true);
		return;
	}
	
	if(bWaiting)
	{
		UE_LOG(LogMultiplayerGameInstance, Error, TEXT("Already waiting until the module is ready for another callback."))
		Callback(false);
		return;
	}
	
	WaitUntilReadyCallback = Callback;

	
	//
	// All processes to wait for. The InitializeCount should equal the amount of processes here.
	//

	// Login to connect-interface.
	UConnectSubsystem* ConnectSubsystem = GetSubsystem<UConnectSubsystem>();
	DelegateHandles.Add(ConnectSubsystem->OnConnectLoginCompleteDelegate.AddUFunction(this, FName("WaitProcessComplete")));
	ConnectSubsystem->Login();

	// todo: auth-interface. todo: others async operations here.
}

/*
 * Will decrease the InitializeCount and call the WaitUntilReadyCallback when it reached zero.
 */
void UMultiplayerGameInstance::WaitProcessComplete() {
	if(--InitializeCount == 0 && WaitUntilReadyCallback)
	{
		UE_LOG(LogMultiplayerGameInstance, Log, TEXT("Async operations complete. Module is now ready to be used."))
		UConnectSubsystem* ConnectSubsystem = GetSubsystem<UConnectSubsystem>();
		for (const FDelegateHandle Handle : DelegateHandles) ConnectSubsystem->OnConnectLoginCompleteDelegate.Remove(Handle);

		DelegateHandles.Empty();
		bWaiting = false;
		WaitUntilReadyCallback(true);
		WaitUntilReadyCallback = nullptr;
	}
}
