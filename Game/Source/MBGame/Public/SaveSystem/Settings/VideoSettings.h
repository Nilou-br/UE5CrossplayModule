// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "VideoSettings.generated.h"



/**
 Video Settings
 */
UCLASS(BlueprintType, Config = VideoSettings)
class MBGAME_API UVideoSettings : public UObject
{
	GENERATED_BODY()

public:
	UVideoSettings();
};