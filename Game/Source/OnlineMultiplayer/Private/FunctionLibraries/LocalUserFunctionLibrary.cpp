// Copyright © 2023 Melvin Brink

#include "FunctionLibraries/LocalUserFunctionLibrary.h"
#include "Subsystems/User/Local/LocalUserSubsystem.h"



ULocalUser* ULocalUserFunctionLibrary::GetLocalUser(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return nullptr;

	const ULocalUserSubsystem* LocalUserSubsystem = World->GetGameInstance()->GetSubsystem<ULocalUserSubsystem>();
	return LocalUserSubsystem->GetLocalUser();
}
