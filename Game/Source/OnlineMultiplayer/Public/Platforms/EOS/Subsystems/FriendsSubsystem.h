﻿// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "eos_sdk.h"
#include "UserTypes.h"
#include "FriendsSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFriendsSubsystem, Log, All);
inline DEFINE_LOG_CATEGORY(LogFriendsSubsystem);



/**
 * Subsystem for managing friends.
 */
UCLASS(BlueprintType)
class ONLINEMULTIPLAYER_API UFriendsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	UFriendsSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION(BlueprintCallable)
	TArray<UPlatformUser*> GetFriendList() const;

	UFUNCTION(BlueprintCallable)
	void InviteToLobby(UPlatformUser* PlatformUser);

private:
	class FEosManager* EosManager;
	EOS_HFriends FriendsHandle;

	UPROPERTY() class USteamFriendsSubsystem* SteamFriendsSubsystem;
	UPROPERTY() class ULocalUserSubsystem* LocalUserSubsystem;
};
