// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionList.generated.h"


class UButton;
class USessionListItem;

UCLASS()
class MBGAME_API USessionList : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Session")
	void TestFindSessions();

private:
	// UPROPERTY(meta = (BindWidget))
	// UButton* HostGameButton;
	// UPROPERTY(meta = (BindWidget))
	// UButton* JoinAnyGameButton;
	//
	// // Button Click Events
	// UFUNCTION()
	// void HostGameButtonClicked();
	// UFUNCTION()
	// void JoinAnyGameButtonClicked();


	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SessionListScrollBox;
	
	// Callbacks
	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FString Address, EOnJoinSessionCompleteResult::Type Result);
	void OnFindSessionsComplete(bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionResults);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USessionListItem> SessionListItemClass;

	UPROPERTY(NoClear)
	class USessionManager* SessionManager;
};
