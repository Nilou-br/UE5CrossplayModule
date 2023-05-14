// Copyright © 2023 Melvin Brink

#include "MBGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "EOS/Subsystems/LocalUserStateSubsystem.h"


void UMBGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogMBGameInstance, Log, TEXT("Initializing MBGameInstance"));

	// TODO: Compatibility for other platforms.
	// Set the platform type for the local user.
	ULocalUserStateSubsystem* LocalUserStateSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<ULocalUserStateSubsystem>();
	LocalUserStateSubsystem->SetPlatformType(EPlatformType::PlatformType_Steam);
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
