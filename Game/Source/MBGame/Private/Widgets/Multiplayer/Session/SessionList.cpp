// Copyright © 2023 Melvin Brink


#include "Widgets/Multiplayer/Session/SessionList.h"
#include "Widgets/Multiplayer/Session/SessionListItem.h"
#include "Multiplayer/SessionManager.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "OnlineSessionSettings.h"



void USessionList::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize the SessionManager
	if(const UGameInstance* GameInstance = GetGameInstance(); GameInstance) SessionManager = GameInstance->GetSubsystem<USessionManager>();

	// Bind callbacks to delegates
	if(SessionManager)
	{
		SessionManager->ManagerOnCreateSessionComplete.AddUObject(this, &ThisClass::OnCreateSessionComplete);
		SessionManager->ManagerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
		SessionManager->ManagerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsComplete);
	}

	// if(HostGameButton) HostGameButton->OnClicked.AddDynamic(this, &ThisClass::HostGameButtonClicked);
	// if(HostGameButton) JoinAnyGameButton->OnClicked.AddDynamic(this, &ThisClass::JoinAnyGameButtonClicked);
}



void USessionList::TestFindSessions()
{
	if (SessionManager) SessionManager->FindSessions();
}


void USessionList::OnCreateSessionComplete(bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("USessionList::OnCreateSessionComplete: bWasSuccessful is false"))
		return;
	}

	// Open session widget.
}


void USessionList::OnJoinSessionComplete(FString Address, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenu OnJoinSessionComplete: Failed"))
		// Display error message to user.
		return;
	}

	// Open session widget.
	
}


void USessionList::OnFindSessionsComplete(bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionResults)
{
	if(!SessionManager || !bWasSuccessful)
	{
		if(!bWasSuccessful) UE_LOG(LogTemp, Warning, TEXT("MainMenu OnFindSessionsComplete: bWasSuccessful is false"))
		return;
	}



	// Empty the ScrollBox.
	SessionListScrollBox->ClearChildren();
	UE_LOG(LogTemp, Warning, TEXT("SessionListScrollBox ClearChildren"))
	
	// Add SessionListItems to the ScrollBox.
	for(auto& Result : SessionResults)
	{
		if(!SessionListItemClass) return;
		UE_LOG(LogTemp, Warning, TEXT("for loop item"))

		USessionListItem* SessionListItem = CreateWidget<USessionListItem>(GetOwningPlayer(), SessionListItemClass);
		if (SessionListItem)
		{
			FBlueprintSessionResult BlueprintSessionResult;
			BlueprintSessionResult.OnlineResult = Result;
			SessionListItem->SetSessionResult(BlueprintSessionResult);
			SessionListScrollBox->AddChild(SessionListItem);
		}
	}

	// JoinAnyGameButton->SetIsEnabled(true);
}