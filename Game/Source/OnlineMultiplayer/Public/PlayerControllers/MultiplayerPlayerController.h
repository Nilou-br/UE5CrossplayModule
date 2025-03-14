// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerPlayerController.generated.h"

/**
 * Custom player controllers should derive from this class.
 */
UCLASS()
class ONLINEMULTIPLAYER_API AMultiplayerPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMultiplayerPlayerController();

	virtual void OnRep_PlayerState() override;
};
