// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "OnlineSettings.generated.h"



/**
 Online Settings
 */
UCLASS(BlueprintType, Config = OnlineSettings)
class MBGAME_API UOnlineSettings : public UObject
{
	GENERATED_BODY()

public:
	UOnlineSettings();

private:
	UPROPERTY(Config)
	bool bCrossPlayEnabled;

public:
	UFUNCTION(BlueprintPure, Category = "Settings")
	FORCEINLINE bool GetCrossPlayEnabled() const { return bCrossPlayEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Settings")
	FORCEINLINE void SetCrossPlayEnabled(bool bEnabled) { bCrossPlayEnabled = bEnabled; }
};