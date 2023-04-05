// Copyright © 2023 Melvin Brink


#include "Widgets/Multiplayer/MultiplayerMenu.h"
#include "Multiplayer/SessionManager.h"



void UMultiplayerMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize the SessionManager.
	if(const UGameInstance* GameInstance = GetGameInstance(); GameInstance) SessionManager = GameInstance->GetSubsystem<USessionManager>();

	// Bind callbacks.
	if(SessionManager)
	{
		SessionManager->ManagerOnCreateSessionComplete.AddUObject(this, &ThisClass::OnCreateSessionComplete);
	}
}



void UMultiplayerMenu::CreateSession()
{
	if(SessionManager) SessionManager->CreateSession();
	UE_LOG(LogTemp, Warning, TEXT("UMultiplayerMenu::CreateSession"));
}


void UMultiplayerMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMultiplayerMenu::OnCreateSessionComplete: bWasSuccessful is false"))
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UMultiplayerMenu::OnCreateSessionComplete: bWasSuccessful is true"))
}

