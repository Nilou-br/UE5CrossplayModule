// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOSPlus/Private/OnlineSessionEOSPlus.h"
#include "OnlineSubsystemEOSPlus/Private/OnlineSubsystemEOSPlus.h"
#include "OnlineManagerProxySubsystem.generated.h"



/**
 Proxy subsystem for handling Sessions and Parties.
 */
UCLASS(BlueprintType)
class MBGAME_API UOnlineManagerProxySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
public:
    UFUNCTION(BlueprintCallable, Category = "Online|Account")
    void Login();
    UFUNCTION(BlueprintCallable, Category = "Online|Account")
    void Logout();

    // Consider using booleans for the create and join functions.
    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    void CreateSession();
    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    void JoinSession();

private:
    void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
    void OnLogoutComplete(int32 LocalUserNum, bool bWasSuccessful);
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
    
    UPROPERTY()
    class USessionManager* SessionManager;
    UPROPERTY()
    class UPartyManager* PartyManager;
    
    IOnlineSubsystem* OSSBase;
    FOnlineSubsystemEOSPlusPtr OSSPlus;
    FOnlineSessionEOSPlusPtr OSSPlusSessionInterface;
    
    FUniqueNetIdPtr UniquePlayerId;
};