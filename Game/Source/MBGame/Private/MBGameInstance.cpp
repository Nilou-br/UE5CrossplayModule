// Copyright © 2023 Melvin Brink

#include "MBGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UserStateSubsystem.h"


void UMBGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogMBGameInstance, Log, TEXT("Initializing MBGameInstance"));

	// TODO: Compatibility for other platforms.
	// Set the platform type for the local user.
	UUserStateSubsystem* UserStateSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUserStateSubsystem>();
	UserStateSubsystem->SetPlatform(EOS_EExternalAccountType::EOS_EAT_STEAM);
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
