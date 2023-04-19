// Copyright © 2023 Melvin Brink

#include "MBGameInstance.h"

#include "Blueprint/UserWidget.h"


void UMBGameInstance::Init()
{
	Super::Init();

	// Create the PartyOverlayWidget.
	// UE_LOG(LogClass, Warning, TEXT("UMBGameInstance::Init"));
	// if (PartyOverlayWidgetClass)
	// {
	// 	UE_LOG(LogClass, Warning, TEXT("Creating PartyOverlayWidget..."));
	// 	PartyOverlayWidget = CreateWidget<UUserWidget>(this, PartyOverlayWidgetClass);
	// 	PartyOverlayWidget->Initialize();
	// }
}

UUserWidget* UMBGameInstance::GetPartyOverlayWidget()
{
	if (!PartyOverlayWidget && PartyOverlayWidgetClass)
	{
		PartyOverlayWidget = CreateWidget<UUserWidget>(this, PartyOverlayWidgetClass);
	}
	return PartyOverlayWidget;
}
