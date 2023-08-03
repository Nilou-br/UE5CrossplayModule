// Copyright © 2023 Melvin Brink

#include "Subsystems/User/Local/BP/LocalUserFunctionLibrary.h"
#include "Subsystems/User/Local/LocalUserSubsystem.h"


ULocalUser* ULocalUserFunctionLibrary::GetLocalUser()
{
	if(!GEngine || !GEngine->GameViewport) return nullptr;
	if(const FWorldContext* WorldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport); WorldContext)
	{
		const ULocalUserSubsystem* LocalUserSubsystem = WorldContext->World()->GetGameInstance()->GetSubsystem<ULocalUserSubsystem>();
		return LocalUserSubsystem->GetLocalUser();
	}
return nullptr;
}
