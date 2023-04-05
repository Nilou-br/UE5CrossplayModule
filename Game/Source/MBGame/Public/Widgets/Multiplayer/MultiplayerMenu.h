// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class MBGAME_API UMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	virtual void NativeConstruct() override;

	
public:
	UFUNCTION(BlueprintCallable)
	void CreateSession();

	
private:
	void OnCreateSessionComplete(bool bWasSuccessful);
	
	UPROPERTY(NoClear)
	class USessionManager* SessionManager;
};
