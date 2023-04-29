// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "eos_sdk.h"
#include "SteamworksSubsystem.h"
#include "EOSSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEOSSubsystem, Log, All);
inline DEFINE_LOG_CATEGORY(LogEOSSubsystem);



/**
 * The EOS Subsystem that handles all EOS related functionality.
 *
 * Use this subsystem to login, logout, create and join sessions, etc.
 */
UCLASS(BlueprintType)
class MBGAME_API UEOSSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
    GENERATED_BODY()
    friend class UMBGameInstance; // Remove later
    friend class FEOSGameModule;

protected:
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    void InitializeEosSdk();
    void InitializeEosPlatform();
    EOS_EResult CreateIntegratedPlatform(EOS_Platform_Options& PlatformOptions);
    void FreeIntegratedPlatform(EOS_Platform_Options& PlatformOptions);
    
    void OnEncryptedAppTicketReady();
    std::string GetEncryptedAppTicketSteam();
    std::string GetSessionTicketSteam();
    
public:
    UFUNCTION(BlueprintCallable, Category = "Online|Login|Connect")
    void LoginConnect();
    UFUNCTION(BlueprintCallable, Category = "Online|Login|Connect")
    void LogoutConnect();

    UFUNCTION(BlueprintCallable, Category = "Online|Login|Auth")
    void LoginAuth();
    UFUNCTION(BlueprintCallable, Category = "Online|Login|Auth")
    void LogoutAuth();

    // Consider using booleans for the create and join functions.
    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    void CreateSession();
    UFUNCTION(BlueprintCallable, Category = "Online|Session")
    void JoinSession();

private:
    static void EOS_CALL OnLoginConnectComplete(const EOS_Connect_LoginCallbackInfo* Data);
    void OnLogoutConnectComplete();

    static void EOS_CALL OnLoginAuthComplete(const EOS_Auth_LoginCallbackInfo* Data);
    
    static void EOS_CALL OnCreateSessionComplete(const EOS_Sessions_UpdateSessionCallbackInfo* Data);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    UPROPERTY()
    USteamworksSubsystem* SteamworksSubsystem;
    
    EOS_HPlatform EosPlatformHandle;
    EOS_ProductUserId EosProductUserId;
    EOS_ContinuanceToken EosContinuanceToken;
    
};