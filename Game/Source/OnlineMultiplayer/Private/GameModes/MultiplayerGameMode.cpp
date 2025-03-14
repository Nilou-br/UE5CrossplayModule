// Copyright © 2023 Melvin Brink

#include "GameModes/MultiplayerGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerStates/MultiplayerPlayerState.h"


AMultiplayerGameMode::AMultiplayerGameMode()
{
	PlayerStateClass = AMultiplayerPlayerState::StaticClass();
}

FString AMultiplayerGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	FString RetString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	if(!NewPlayerController->IsLocalController() && HasAuthority())
	{
		const FString ProductUserID = UGameplayStatics::ParseOption(Options, TEXT("PUID"));
		UE_LOG(LogGameMode, Warning, TEXT("PUID: '%s'"), *ProductUserID);
		UE_LOG(LogGameMode, Warning, TEXT("FUniqueNetIdRepl: '%s'"), *UniqueId.ToString());
		
		if(AMultiplayerPlayerState* PlayerState = Cast<AMultiplayerPlayerState>(NewPlayerController->PlayerState); PlayerState)
		{
			if(!ProductUserID.IsEmpty()) PlayerState->ProductUserID = ProductUserID;
		}
		else UE_LOG(LogGameMode, Warning, TEXT("The PlayerState used is not derived from 'AMultiplayerPlayerState' and thus cannot set its properties like the Product-User-ID."));
	}
	
	return RetString;
}

void AMultiplayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// AMultiplayerPlayerState* PlayerState = Cast<AMultiplayerPlayerState>(NewPlayer->PlayerState);
	// if(!PlayerState)
	// {
	// 	UE_LOG(LogGameMode, Warning, TEXT("The PlayerState used is not derived from 'AMultiplayerPlayerState' and thus cannot set its properties like the Product-User-ID."));
	// 	return;
	// }
	//
	// if(NewPlayer->IsLocalPlayerController()) PlayerState->InitPlayerState();
}
