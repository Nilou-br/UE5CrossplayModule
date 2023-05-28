﻿// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "eos_sdk.h"
#include "OnlineMultiplayer_CommonTypes.h"
#include "ConnectSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogConnectSubsystem, Log, All);
inline DEFINE_LOG_CATEGORY(LogConnectSubsystem);



/**
 * Subsystem for authorizing users for both Auth and Connect interfaces.
 *
 * Connect is used for multiplayer and matchmaking.
 *
 * Auth is used for user accounts and friends.
 */
UCLASS(BlueprintType)
class ONLINEMULTIPLAYER_API UConnectSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Account")
	void Login();
	UFUNCTION(BlueprintCallable, Category = "Account")
	void Logout();

private:
	static void EOS_CALL OnLoginComplete(const EOS_Connect_LoginCallbackInfo* Data);
	void OnLogoutComplete();

public:
	void GetUserInfo(TArray<EOS_ProductUserId>& UserIDs, const TFunction<void(FOnlineUserMap)> Callback);

private:
	static void EOS_CALL OnGetUserInfoComplete(const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* Data);
	TFunction<void(FOnlineUserMap)> GetUserInfoCallback;
	
	void CreateNewUser();
	void CheckAccounts();

	class FEosManager* EosManager;
	class FSteamManager* SteamManager;
	
	EOS_HConnect ConnectHandle;
	EOS_ProductUserId EosProductUserId;
	EOS_ContinuanceToken EosContinuanceToken;

	UPROPERTY()
	class UUserStateSubsystem* UserState;
	UPROPERTY()
	class UUserSubsystem* UserSubsystem;
};
