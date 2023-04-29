// Copyright © 2023 Melvin Brink

#include "MBGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Online/SteamworksSubsystem.h"
#include "Online/EOSSubsystem.h"



void UMBGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogMBGameInstance, Log, TEXT("Initializing MBGameInstance"));

	SteamworksSubsystem = GetSubsystem<USteamworksSubsystem>();
	EOSSubsystem = GetSubsystem<UEOSSubsystem>();

	if (SteamworksSubsystem && EOSSubsystem)
	{
		UE_LOG(LogMBGameInstance, Log, TEXT("SteamworksSubsystem and EOSSubsystem are valid"));
		
		// Reference for accessing the SteamworksSubsystem from the EOSSubsystem.
		// Could make a setter, but this is only done once.
		EOSSubsystem->SteamworksSubsystem = SteamworksSubsystem;
		
		// Bind the SteamworksSubsystem's OnEncryptedAppTicketReady to the EOSSubsystem's OnEncryptedAppTicketReady.
		// SteamworksSubsystem->OnEncryptedAppTicketReady.AddUObject(EOSSubsystem, &UEOSSubsystem::OnEncryptedAppTicketReady);
		// SteamworksSubsystem->RequestSteamEncryptedAppTicket();
	}
}

void UMBGameInstance::Shutdown()
{
	//
	
	Super::Shutdown();
}



UUserWidget* UMBGameInstance::GetPartyOverlayWidget()
{
	if (!PartyOverlayWidget && PartyOverlayWidgetClass)
	{
		PartyOverlayWidget = CreateWidget<UUserWidget>(this, PartyOverlayWidgetClass);
	}
	return PartyOverlayWidget;
}
