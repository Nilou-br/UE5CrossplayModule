// Copyright © 2023 Melvin Brink


#include "Widgets/Session/SessionList.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "Multiplayer/SessionManager.h"



/*
	Setup the main menu widget.
 */

void USessionList::Setup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	/*// Interact with UI only.
	if(const UWorld* World = GetWorld())
	{
		if(APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}*/

	// Initialize SessionManager
	if(const UGameInstance* GameInstance = GetGameInstance(); GameInstance) SessionManager = GameInstance->GetSubsystem<USessionManager>();

	if(SessionManager)
	{
		SessionManager->ManagerOnCreateSessionComplete.AddUObject(this, &ThisClass::OnCreateSessionComplete);
		SessionManager->ManagerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
		SessionManager->ManagerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsComplete);
	}
}


bool USessionList::Initialize()
{
	if(!Super::Initialize()) return false;

	if(HostGameButton) HostGameButton->OnClicked.AddDynamic(this, &ThisClass::HostGameButtonClicked);
	if(HostGameButton) JoinAnyGameButton->OnClicked.AddDynamic(this, &ThisClass::JoinAnyGameButtonClicked);
	
	return true;
}



/*
	Button Clicked events
 */

void USessionList::HostGameButtonClicked()
{
	HostGameButton->SetIsEnabled(false);
	if(SessionManager) SessionManager->CreateSession();
}


void USessionList::JoinAnyGameButtonClicked()
{
	JoinAnyGameButton->SetIsEnabled(false);
	if(SessionManager) SessionManager->FindSessions();
}



/*
	Callbacks
 */

void USessionList::OnCreateSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("MainMenu OnCreateSessionComplete"))
}


void USessionList::OnJoinSessionComplete(FString Address)
{
	UE_LOG(LogTemp, Warning, TEXT("MainMenu OnJoinSessionComplete"))
	UE_LOG(LogTemp, Warning, TEXT("Address: %s"), *Address)
}


void USessionList::OnFindSessionsComplete(bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionResults)
{
	UE_LOG(LogTemp, Warning, TEXT("MainMenu OnFindSessionsComplete"))
	if(!SessionManager || !bWasSuccessful || SessionResults.Num() <= 0)
	{
		if(!bWasSuccessful) UE_LOG(LogTemp, Warning, TEXT("MainMenu OnFindSessionsComplete: bWasSuccessful is false"))
		if(SessionResults.Num() <= 0) UE_LOG(LogTemp, Warning, TEXT("MainMenu OnFindSessionsComplete: SessionResults.Num() is 0"))
		JoinAnyGameButton->SetIsEnabled(true);
		return;
	}

	SessionManager->JoinSession(SessionResults[0]);
	return;
	/*for(auto& Result : SessionResults)
	{
		SessionManager->JoinSession(Result);
		return;
	}*/

	JoinAnyGameButton->SetIsEnabled(true);
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("UMainMenu::OnFindSessionsComplete: No sessions found!)"));
}