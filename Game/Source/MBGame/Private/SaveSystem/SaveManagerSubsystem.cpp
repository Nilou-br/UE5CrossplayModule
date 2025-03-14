// Copyright © 2023 Melvin Brink

#include "SaveSystem/SaveManagerSubsystem.h"
#include "SaveSystem/Settings/OnlineSettings.h"
#include "SaveSystem/Settings/ControlSettings.h"
#include "SaveSystem/Settings/VideoSettings.h"
#include "SaveSystem/Settings/SoundSettings.h"
#include "SaveSystem/Saves/GameSave.h"
#include "Kismet/GameplayStatics.h"



void USaveManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load all the settings on startup to prevent having to load them when we need to use them.
	LoadOnlineSettings();
}



// Online Settings.
void USaveManagerSubsystem::SaveOnlineSettings(UOnlineSettings* Settings)
{
	UE_LOG(LogTemp, Warning, TEXT("Saving Online settings"));
	Settings->TryUpdateDefaultConfigFile(ConfigFilePath);
}

UOnlineSettings* USaveManagerSubsystem::LoadOnlineSettings()
{
	if (LoadedOnlineSettings) return LoadedOnlineSettings;
	LoadedOnlineSettings = NewObject<UOnlineSettings>(this, UOnlineSettings::StaticClass());
	LoadedOnlineSettings->LoadConfig(UOnlineSettings::StaticClass(), *ConfigFilePath);
	return LoadedOnlineSettings;
}


// Control Settings.
void USaveManagerSubsystem::SaveControlSettings(UControlSettings* Settings)
{
	UE_LOG(LogTemp, Warning, TEXT("Saving Control settings"));
	Settings->TryUpdateDefaultConfigFile();
}

UControlSettings* USaveManagerSubsystem::LoadControlSettings()
{
	if (LoadedControlSettings) return LoadedControlSettings;
	LoadedControlSettings = NewObject<UControlSettings>(this, UControlSettings::StaticClass());
	LoadedControlSettings->LoadConfig();
	return LoadedControlSettings;
}


// Video Settings.
void USaveManagerSubsystem::SaveVideoSettings(UVideoSettings* Settings)
{
	UE_LOG(LogTemp, Warning, TEXT("Saving Video settings"));
	Settings->TryUpdateDefaultConfigFile();
}

UVideoSettings* USaveManagerSubsystem::LoadVideoSettings()
{
	if (LoadedVideoSettings) return LoadedVideoSettings;
	LoadedVideoSettings = NewObject<UVideoSettings>(this, UVideoSettings::StaticClass());
	LoadedVideoSettings->LoadConfig();
	return LoadedVideoSettings;
}


// Audio Settings.
void USaveManagerSubsystem::SaveSoundSettings(USoundSettings* Settings)
{
	UE_LOG(LogTemp, Warning, TEXT("Saving Sound settings"));
	Settings->TryUpdateDefaultConfigFile();
}

USoundSettings* USaveManagerSubsystem::LoadSoundSettings()
{
	if (LoadedSoundSettings) return LoadedSoundSettings;
	LoadedSoundSettings = NewObject<USoundSettings>(this, USoundSettings::StaticClass());
	LoadedSoundSettings->LoadConfig();
	return LoadedSoundSettings;
}


// Game Saves.
void USaveManagerSubsystem::SaveGameData(FString SaveName, UGameSave* SaveData)
{
	UGameplayStatics::SaveGameToSlot(SaveData, SaveName, 0);
}

UGameSave* USaveManagerSubsystem::LoadGameData(FString SaveName)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		return Cast<UGameSave>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
	}
    
	// If the game mode save doesn't exist, create a new one with default values
	return Cast<UGameSave>(UGameplayStatics::CreateSaveGameObject(UGameSave::StaticClass()));
}