// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "SoundSettings.generated.h"



/**
 Audio Settings
 
 (Named SoundSettings to avoid confusion with the AudioSettings class)
 */
UCLASS(BlueprintType, Config = AudioSettings)
class MBGAME_API USoundSettings : public UObject
{
	GENERATED_BODY()

public:
	USoundSettings();
};