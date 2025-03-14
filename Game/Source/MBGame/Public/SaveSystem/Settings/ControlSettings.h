// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "ControlSettings.generated.h"



/**
 Control Settings
 */
UCLASS(BlueprintType, Config = ControlSettings)
class MBGAME_API UControlSettings : public UObject
{
	GENERATED_BODY()

public:
	UControlSettings();
};