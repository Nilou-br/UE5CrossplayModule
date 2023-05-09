// Copyright © 2023 Melvin Brink

#include "MBGameInstance.h"
#include "Blueprint/UserWidget.h"



void UMBGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogMBGameInstance, Log, TEXT("Initializing MBGameInstance"));
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
