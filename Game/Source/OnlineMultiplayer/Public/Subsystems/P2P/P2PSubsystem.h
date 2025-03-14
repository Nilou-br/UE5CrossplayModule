// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "eos_sdk.h"
#include "P2PSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEMULTIPLAYER_API UP2PSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UP2PSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static void OnConnectionRequest(const EOS_P2P_OnIncomingConnectionRequestInfo* Data);

	// EOS Variables
	EOS_HP2P P2PHandle;
	EOS_NotificationId DirectMessageRequestNotification;
	EOS_NotificationId LobbyMessageRequestNotification;

	// Subsystems
	class FEosManager* EosManager;
	UPROPERTY() class ULocalUserSubsystem* LocalUserSubsystem;
};
