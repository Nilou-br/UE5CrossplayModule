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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void InitLocalPlayer();
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FString ProductUserID;
	
	UFUNCTION(Server, Reliable)
	void ServerSetProductUserID(const FString& InProductUserID);
	void ServerSetProductUserID_Implementation(const FString& InProductUserID) { ProductUserID = InProductUserID; }
};
