// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Platforms/EOS/Subsystems/ConnectSubsystem.h"
#include "MBGameInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMBGameInstance, Log, All);
inline DEFINE_LOG_CATEGORY(LogMBGameInstance);



/**
 * 
 */
UCLASS()
class MBGAME_API UMBGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	uint8 PendingOperations;
	void StartupGame();

public:
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void HideLoadingScreen();

	UFUNCTION()
	void OnConnectLoginComplete(const bool bSuccess, UEosUser* LocalUser);
	
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	UUserWidget* GetPartyOverlayWidget() const;

private:
	// Loading-Screen Widget.
	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;
	UPROPERTY()
	UUserWidget* LoadingScreenWidget;
	
	// Party Widget.
	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> LobbyOverlayWidgetClass;
	UPROPERTY()
	UUserWidget* LobbyOverlayWidget;
};
