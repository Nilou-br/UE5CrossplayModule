// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEMULTIPLAYER_API AMultiplayerGameMode : public AGameMode
{
	GENERATED_BODY()
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
