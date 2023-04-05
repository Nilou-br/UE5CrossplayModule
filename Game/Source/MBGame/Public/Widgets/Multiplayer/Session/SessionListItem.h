// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindSessionsCallbackProxy.h"
#include "SessionListItem.generated.h"

/**
 * 
 */
UCLASS()
class MBGAME_API USessionListItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Session")
	FORCEINLINE void SetSessionResult(const FBlueprintSessionResult InSessionResult) { SessionResult = InSessionResult; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FBlueprintSessionResult SessionResult;
};
