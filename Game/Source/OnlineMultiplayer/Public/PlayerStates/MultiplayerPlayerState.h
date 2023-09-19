// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerPlayerState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMPPlayerState, Log, All);
inline DEFINE_LOG_CATEGORY(LogMPPlayerState);



/**
 * Player state that automatically sets variables such as the 'username' or the 'product user ID', and replicates it.
 *
 * Custom player states should derive from this class.
 */
UCLASS()
class ONLINEMULTIPLAYER_API AMultiplayerPlayerState : public APlayerState
{
	GENERATED_BODY()

	AMultiplayerPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(Replicated, BlueprintReadOnly, ReplicatedUsing=OnRep_ProductUserID)
	FString ProductUserID;

	UFUNCTION()
	void OnRep_ProductUserID();
	
	UFUNCTION(Server, Reliable)
	void ServerSetProductUserID(const FString& InProductUserID);
	void ServerSetProductUserID_Implementation(const FString& InProductUserID) { ProductUserID = InProductUserID; }


	UPROPERTY(BlueprintReadOnly)
	FString Username;
};
