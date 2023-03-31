// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionList.generated.h"


class UButton;

UCLASS()
class MBGAME_API USessionList : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Setup();

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* HostGameButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinAnyGameButton;

	// Button Click Events
	UFUNCTION()
	void HostGameButtonClicked();
	UFUNCTION()
	void JoinAnyGameButtonClicked();

	// Callbacks
	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FString Address);
	void OnFindSessionsComplete(bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionResults);

	UPROPERTY(NoClear)
	class USessionManager* SessionManager;
};
