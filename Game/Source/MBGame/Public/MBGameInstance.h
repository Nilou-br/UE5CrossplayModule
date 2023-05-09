// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

public:
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	UUserWidget* GetPartyOverlayWidget();

private:
	class FSteamManager* SteamManager;

	// Party Widget.
	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PartyOverlayWidgetClass;
	UPROPERTY()
	UUserWidget* PartyOverlayWidget;
};
