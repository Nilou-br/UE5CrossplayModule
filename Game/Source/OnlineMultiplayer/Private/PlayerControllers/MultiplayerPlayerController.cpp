// Copyright © 2023 Melvin Brink

#include "PlayerControllers/MultiplayerPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/User/Local/LocalUserSubsystem.h"


AMultiplayerPlayerController::AMultiplayerPlayerController()
{
	bReplicates = true;
}

void AMultiplayerPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ProductUserID);
}

void AMultiplayerPlayerController::InitLocalPlayer()
{
	const ULocalUserSubsystem* LocalUserSubsystem = GetGameInstance()->GetSubsystem<ULocalUserSubsystem>();
	const FString LocalProductUserID = LocalUserSubsystem->GetLocalUser()->GetProductUserID();

	UE_LOG(LogTemp, Warning, TEXT("::InitLocalPlayer ProductUserID is: '%s'"), *LocalProductUserID);
    
	// Call the server function to replicate the ID
	ServerSetProductUserID(LocalProductUserID);
}
