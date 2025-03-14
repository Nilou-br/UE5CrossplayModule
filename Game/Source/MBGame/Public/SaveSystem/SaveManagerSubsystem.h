// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManagerSubsystem.generated.h"



/*
 Manages all config/save data.
  
 Handles settings and game saves differently.
 - Settings are saved in a single .ini config file.
 - Game Saves are saved per game type.

 Config Setting types:
 - Online settings
 - Control settings
 - Video settings
 - Sound settings
 */
UCLASS(BlueprintType)
class MBGAME_API USaveManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	// Online Settings.
	UFUNCTION(BlueprintCallable, Category="Settings")
	void SaveOnlineSettings(class UOnlineSettings* Settings);
	UFUNCTION(BlueprintCallable, Category="Settings")
	UOnlineSettings* LoadOnlineSettings();

	// Control Settings.
	UFUNCTION(BlueprintCallable, Category="Settings")
	void SaveControlSettings(class UControlSettings* Settings);
	UFUNCTION(BlueprintCallable, Category="Settings")
	UControlSettings* LoadControlSettings();

	// Video Settings.
	UFUNCTION(BlueprintCallable, Category="Settings")
	void SaveVideoSettings(class UVideoSettings* Settings);
	UFUNCTION(BlueprintCallable, Category="Settings")
	UVideoSettings* LoadVideoSettings();

	// Audio Settings.
	UFUNCTION(BlueprintCallable, Category="Settings")
	void SaveSoundSettings(class USoundSettings* Settings);
	UFUNCTION(BlueprintCallable, Category="Settings")
	USoundSettings* LoadSoundSettings();
	
	
	
	// Game Saves.
	UFUNCTION(BlueprintCallable, Category="Game")
	void SaveGameData(FString SaveName, class UGameSave* SaveData);
	UGameSave* LoadGameData(FString SaveName);

private:
	UPROPERTY()
	UOnlineSettings* LoadedOnlineSettings;
	UPROPERTY()
	UControlSettings* LoadedControlSettings;
	UPROPERTY()
	UVideoSettings* LoadedVideoSettings;
	UPROPERTY()
	USoundSettings* LoadedSoundSettings;

	const FString ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() / TEXT("UserSettings.ini"));
	const FString SettingsSaveSlotName = TEXT("Settings");
	const FString GameSaveSlotName = TEXT("Game");
};
