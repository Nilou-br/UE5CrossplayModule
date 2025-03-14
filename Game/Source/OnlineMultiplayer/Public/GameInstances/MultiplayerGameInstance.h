// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiplayerGameInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMultiplayerGameInstance, Log, All);
inline DEFINE_LOG_CATEGORY(LogMultiplayerGameInstance);



/**
 * Base game-instance class for the Cross-Play module.
 */
UCLASS()
class ONLINEMULTIPLAYER_API UMultiplayerGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	void WaitUntilReady(TFunction<void(bool bWasSuccessful)>const& Callback);

private:
	uint8 InitializeCount = 1; // Number of processes to wait for.
	TArray<FDelegateHandle> DelegateHandles;
	bool bWaiting = false;
	TFunction<void(bool bWasSuccessful)> WaitUntilReadyCallback;
	UFUNCTION()void WaitProcessComplete();
};
