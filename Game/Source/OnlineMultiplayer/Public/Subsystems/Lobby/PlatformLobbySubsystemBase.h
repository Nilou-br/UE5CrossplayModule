// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "PlatformLobbySubsystemBase.generated.h"



/*
 * Information about this lobby
 */
USTRUCT(BlueprintType)
struct FShadowLobbyDetails
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString LobbyID;

	UPROPERTY(BlueprintReadOnly)
	FString LobbyOwnerID;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, class UOnlineUser*> MemberList;
};

UENUM(BlueprintType)
enum class EShadowLobbyResultCode : uint8
{
	Success UMETA(DisplayName = "Success."),
	CreateFailure UMETA(DisplayName = "Failed to create lobby."),
	JoinFailure UMETA(DisplayName = "Failed to join lobby."),
	InLobby UMETA(DisplayName = "Already in lobby."),
	Unknown UMETA(DisplayName = "Unkown error occurred."),
};

USTRUCT(BlueprintType)
struct FShadowLobbyResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FShadowLobbyDetails LobbyDetails;

	UPROPERTY(BlueprintReadOnly)
	EShadowLobbyResultCode ResultCode;
};



/*
 * Base class for the platform-lobby-subsystems
 */
UCLASS()
class ONLINEMULTIPLAYER_API UPlatformLobbySubsystemBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnCreateShadowLobbyCompleteDelegate, FShadowLobbyResult);
	DECLARE_DELEGATE_OneParam(FOnJoinShadowLobbyCompleteDelegate, FShadowLobbyResult);
	DECLARE_DELEGATE_OneParam(FOnUserJoinShadowLobbyDelegate, FString);
	DECLARE_DELEGATE_OneParam(FOnUserLeaveShadowLobbyDelegate, FString);

	FOnCreateShadowLobbyCompleteDelegate OnCreateShadowLobbyCompleteDelegate;
	FOnJoinShadowLobbyCompleteDelegate OnJoinShadowLobbyCompleteDelegate;
	
	FOnUserJoinShadowLobbyDelegate OnUserJoinSteamLobbyDelegate;
	FOnUserLeaveShadowLobbyDelegate OnUserLeaveSteamLobbyDelegate;
	
	virtual void CreateLobby(){}
	virtual void JoinLobby(){}
};
