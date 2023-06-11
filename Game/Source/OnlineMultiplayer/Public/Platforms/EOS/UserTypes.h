﻿// Copyright © 2023 Melvin Brink

#pragma once

#include "EOSManager.h"
#include "eos_common.h"
#include "UserTypes.generated.h"



// TODO: UUser class for platform specific. UEosUser : UUser for online users. ULocalUser : UEosUser for some extra local functionality.

// These are the external account types that EOS supports
struct FExternalAccount {
	EOS_ProductUserId ProductUserID;
	FString DisplayName;
	FString AccountID;
	EOS_EExternalAccountType AccountType;
	int64_t LastLoginTime;
};
typedef TMap<EOS_EExternalAccountType, FExternalAccount> FExternalAccountsMap;



/**
 * The platform user contains the data that any platform can supply, such as getting the username or avatar of a Steam or PSN user, and should be used for the friends on the users platform.
 */
UCLASS(BlueprintType)
class UPlatformUser : public UObject
{
	GENERATED_BODY()

	struct FPlatformUserState
	{
		FString PlatformUserID;
		FString DisplayName;
		UTexture2D* Avatar;
	};

protected:
	FPlatformUserState PlatformUserState;

public:
	void Initialize(const FString& InPlatformUserID, const FString& InDisplayName)
	{
		PlatformUserState.PlatformUserID = InPlatformUserID;
		PlatformUserState.DisplayName = InDisplayName;
	}
	
	// Getters
	UFUNCTION(BlueprintCallable) FORCEINLINE FString GetPlatformID() const { return PlatformUserState.PlatformUserID; }
	UFUNCTION(BlueprintCallable) FORCEINLINE FString GetUsername() const { return PlatformUserState.DisplayName; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UTexture2D* GetAvatar() const { return PlatformUserState.Avatar; }

	// Setters
	FORCEINLINE void SetPlatformID(const FString& PlatformUserID) { PlatformUserState.PlatformUserID = PlatformUserID; }
	FORCEINLINE void SetPlatformID(const uint64& PlatformUserID) { PlatformUserState.PlatformUserID = FString::FromInt(PlatformUserID); }
	FORCEINLINE void SetUsername(const FString& DisplayName) { PlatformUserState.DisplayName = DisplayName; }
	FORCEINLINE void SetAvatar(UTexture2D* AvatarURL) { PlatformUserState.Avatar = AvatarURL; }
};



/**
 * Represents an online user which has the extra properties we need to do matchmaking.
 *
 * Users joining a lobby/session will join with their EOS-product-user-ID and we can use that to fetch some data and create this user.
 */
UCLASS(BlueprintType)
class UEosUser : public UPlatformUser
{
	GENERATED_BODY()
	
	struct FUserState
	{
		EOS_ProductUserId ProductUserID;
		EOS_EpicAccountId EpicAccountID;
		FExternalAccountsMap ExternalAccounts;
		EOS_EExternalAccountType Platform;
	};

protected:
	FUserState UserState;
	
public:
	void Initialize(
			const EOS_ProductUserId& InProductUserID,
			const EOS_EpicAccountId& InEpicAccountID,
			const FExternalAccountsMap& InExternalAccounts,
			const EOS_EExternalAccountType InPlatform,
			const FString& InPlatformUserID,
			const FString& InDisplayName)
	{
		UserState.ProductUserID = InProductUserID;
		UserState.EpicAccountID = InEpicAccountID;
		UserState.ExternalAccounts = InExternalAccounts;
		UserState.Platform = InPlatform;
		PlatformUserState.PlatformUserID = InPlatformUserID;
		PlatformUserState.DisplayName = InDisplayName;
	}
	
	// Getters
	FORCEINLINE EOS_ProductUserId GetProductUserID() const { return UserState.ProductUserID; }
	FORCEINLINE EOS_EpicAccountId GetEpicAccountID() const { return UserState.EpicAccountID; }
	FORCEINLINE EOS_EExternalAccountType GetPlatform() const { return UserState.Platform; }

	// Setters
	FORCEINLINE void SetProductUserID(const EOS_ProductUserId& InProductUserID) { UserState.ProductUserID = InProductUserID; }
	FORCEINLINE void SetEpicAccountID(const EOS_EpicAccountId& InEpicAccountID) { UserState.EpicAccountID = InEpicAccountID; }
	FORCEINLINE void SetPlatform(const EOS_EExternalAccountType PlatformType) { UserState.Platform = PlatformType; }
};


/**
 * The local user is an EOS user with some extra data and helper methods.
 */
UCLASS(BlueprintType)
class ULocalUser final : public UEosUser
{
	GENERATED_BODY()
	
	struct FLocalUserState
	{
		FString LobbyID;
		FString ShadowLobbyID;
		EOS_ContinuanceToken ContinuanceToken;
	};
	FLocalUserState LocalUserState;
	
public:
	// Getters
	FORCEINLINE FString GetLobbyID() const { return LocalUserState.LobbyID; }
	FORCEINLINE FString GetShadowLobbyID() const { return LocalUserState.ShadowLobbyID; }
	FORCEINLINE EOS_ContinuanceToken GetContinuanceToken() const { return LocalUserState.ContinuanceToken; }

	// Setters
	FORCEINLINE void SetLobbyID(const FString& InLobbyID) { LocalUserState.LobbyID = InLobbyID; }
	FORCEINLINE void SetLobbyID(const uint64& InLobbyID) { LocalUserState.LobbyID = FString::FromInt(InLobbyID); }
	FORCEINLINE void SetShadowLobbyID(const FString& InShadowLobbyID) { LocalUserState.ShadowLobbyID = InShadowLobbyID; }
	FORCEINLINE void SetShadowLobbyID(const uint64& InShadowLobbyID) { LocalUserState.ShadowLobbyID = FString::FromInt(InShadowLobbyID); }
	FORCEINLINE void SetContinuanceToken(const EOS_ContinuanceToken& InContinuanceToken) { LocalUserState.ContinuanceToken = InContinuanceToken; }

	// Helper functions to make the code more readable
	FORCEINLINE bool IsAuthLoggedIn() const { return EOS_EpicAccountId_IsValid(UserState.EpicAccountID) == EOS_TRUE; }
	FORCEINLINE bool IsConnectLoggedIn() const { return EOS_ProductUserId_IsValid(UserState.ProductUserID) == EOS_TRUE; }
	FORCEINLINE bool IsInLobby() const { return !LocalUserState.LobbyID.IsEmpty(); }
	FORCEINLINE bool IsInShadowLobby() const { return !LocalUserState.ShadowLobbyID.IsEmpty(); }
};





// --- Global helper functions ---

inline FString EosIDToString(const EOS_ProductUserId& ProductUserID)
{
	char IDBuffer[EOS_PRODUCTUSERID_MAX_LENGTH];
	int32_t BufferSize = EOS_PRODUCTUSERID_MAX_LENGTH;
	if(const EOS_EResult ResultCode = EOS_ProductUserId_ToString(ProductUserID, IDBuffer, &BufferSize); ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEos, Warning, TEXT("EOS_ProductUserId_ToString failed. Result Code: [%s]"), *FString(EOS_EResult_ToString(ResultCode)));
		return FString();
	}
	return IDBuffer;
}

inline FString EosIDToString(const EOS_EpicAccountId& EpicAccountID)
{
	char IDBuffer[EOS_PRODUCTUSERID_MAX_LENGTH];
	int32_t BufferSize = EOS_PRODUCTUSERID_MAX_LENGTH;
	if(const EOS_EResult ResultCode = EOS_EpicAccountId_ToString(EpicAccountID, IDBuffer, &BufferSize); ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEos, Warning, TEXT("EOS_ProductUserId_ToString failed. Result Code: [%s]"), *FString(EOS_EResult_ToString(ResultCode)));
		return FString();
	}
	return IDBuffer;
}