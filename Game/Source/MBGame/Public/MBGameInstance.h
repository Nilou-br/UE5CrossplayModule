// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MBGAME_API UMBGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	UUserWidget* GetPartyOverlayWidget();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PartyOverlayWidgetClass;

	UPROPERTY()
	UUserWidget* PartyOverlayWidget;
	
};
