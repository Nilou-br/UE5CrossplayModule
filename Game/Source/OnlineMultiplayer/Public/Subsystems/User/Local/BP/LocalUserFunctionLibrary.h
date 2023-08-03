// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LocalUserFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEMULTIPLAYER_API ULocalUserFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category="User|Local")
	static class ULocalUser* GetLocalUser();
};
