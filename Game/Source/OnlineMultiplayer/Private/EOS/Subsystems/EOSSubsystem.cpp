// Copyright © 2023 Melvin Brink

#include "EOS/Subsystems/EOSSubsystem.h"

#include <string>
#include "eos_sdk.h"
#include "eos_logging.h"
#include "eos_common.h"
#include "eos_connect_types.h"
#include "eos_auth.h"
#include "eos_connect.h"
#include "eos_sessions.h"
#include "eos_sessions_types.h"
#include "eos_integratedplatform.h"

/**
 * Receives useful logs from the EOS SDK.
 */
static void EOS_CALL OnEosLogMessage(const EOS_LogMessage* Message)
{
	// Temporary solution.
	if (Message)
	{
		switch (Message->Level)
		{
		case EOS_ELogLevel::EOS_LOG_Fatal:
			UE_LOG(LogEOSSubsystem, Fatal, TEXT("EOS [%hs]: %hs"), Message->Category, Message->Message);
			break;
		case EOS_ELogLevel::EOS_LOG_Error:
			UE_LOG(LogEOSSubsystem, Error, TEXT("EOS [%hs]: %hs"), Message->Category, Message->Message);
			break;
		case EOS_ELogLevel::EOS_LOG_Warning:
			UE_LOG(LogEOSSubsystem, Warning, TEXT("EOS [%hs]: %hs"), Message->Category, Message->Message);
			break;
		case EOS_ELogLevel::EOS_LOG_Info:
			UE_LOG(LogEOSSubsystem, Log, TEXT("EOS [%hs]: %hs"), Message->Category, Message->Message);
			break;
		case EOS_ELogLevel::EOS_LOG_Verbose:
		case EOS_ELogLevel::EOS_LOG_VeryVerbose:
			UE_LOG(LogEOSSubsystem, Verbose, TEXT("EOS [%hs]: %hs"), Message->Category, Message->Message);
			break;
		case EOS_ELogLevel::EOS_LOG_Off:
		default:
			break;
		}
	}
}

void UEOSSubsystem::Tick(float DeltaTime)
{
	if (EosPlatformHandle) EOS_Platform_Tick(EosPlatformHandle); // If-statement maybe redundant because of IsTickable().
}

bool UEOSSubsystem::IsTickable() const
{
	return EosPlatformHandle != nullptr;
}

TStatId UEOSSubsystem::GetStatId() const
{
	// Return an empty TStatId (used for profiling)
	return TStatId();
}


// --------------------------------
// Start of subsystem implementation.


void UEOSSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Register the logging callback.
	EOS_Logging_SetCallback(&OnEosLogMessage);
	EOS_Logging_SetLogLevel(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_Verbose);
	
	// Initialize the EOS-SDK.
	InitializeEosSdk();

	// Initialize the platform.
	InitializeEosPlatform();
}

/**
 * Initializes the EOS-SDK.
 */
void UEOSSubsystem::InitializeEosSdk()
{
	EOS_InitializeOptions InitOptions;
	InitOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	InitOptions.AllocateMemoryFunction = nullptr;
	InitOptions.ReallocateMemoryFunction = nullptr;
	InitOptions.ReleaseMemoryFunction = nullptr;
	InitOptions.ProductName = "MBGame";
	InitOptions.ProductVersion = "0.0.1";
	InitOptions.Reserved = nullptr;
	InitOptions.SystemInitializeOptions = nullptr;
	InitOptions.OverrideThreadAffinity = nullptr;

	const EOS_EResult InitResult = EOS_Initialize(&InitOptions);
	if (InitResult != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to initialize EOS SDK: %s"), *FString(EOS_EResult_ToString(InitResult)));
		return;
	}
	UE_LOG(LogEOSSubsystem, Log, TEXT("EOS SDK initialized successfully"));
}

/**
 * Initializes the EOS-Platform.
 * 
 * Will provide access to all other EOS-SDK interfaces.
 *
 * Sets 'EosPlatformHandle' to a valid handle if successful.
 */
void UEOSSubsystem::InitializeEosPlatform()
{
	// Create the temporary container.
	EOS_Platform_Options PlatformOptions = {};
	if(CreateIntegratedPlatform(PlatformOptions) != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to create Integrated-Platform"));
		return;
	}

	// Set the options.
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.ProductId = "cf6e819c86fc45548c72029b93e59853";
	PlatformOptions.SandboxId = "3bd37bf86718447bbb4f5d7473212ec5";
	PlatformOptions.DeploymentId = "7c5d0e6b46f2429aab485c838008d458";
	PlatformOptions.ClientCredentials.ClientId = "xyza7891cHQIAxVpn4iwHSPLv4JGOZp6";
	PlatformOptions.ClientCredentials.ClientSecret = "RnQd8t2c/OR8qKpQ449re2zln9lheKHL5G9i9x/A5H4";
	PlatformOptions.bIsServer = EOS_FALSE;
	PlatformOptions.EncryptionKey = nullptr;
	PlatformOptions.OverrideCountryCode = nullptr;
	PlatformOptions.OverrideLocaleCode = nullptr;
	PlatformOptions.CacheDirectory = nullptr;
	PlatformOptions.TickBudgetInMilliseconds = 0;
	PlatformOptions.RTCOptions = nullptr;
	PlatformOptions.IntegratedPlatformOptionsContainerHandle = nullptr;
	// PlatformOptions.Flags = EOS_PF_LOADING_IN_EDITOR;

	// Initialize the platform handle.
	EosPlatformHandle = EOS_Platform_Create(&PlatformOptions);
	if (!EosPlatformHandle)
	{
		// Handle error
		UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to initialize EOS Platform"));
		return;
	}
	UE_LOG(LogEOSSubsystem, Log, TEXT("EOS Platform initialized successfully"));

	// Release the temporary container.
	FreeIntegratedPlatform(PlatformOptions);
}

/**
 * Creates a temporary container for the platform-specific options.
 * 
 * Should be released with 'FreeIntegratedPlatform' after use.
 *
 * @param Options The options to configure.
 */
EOS_EResult UEOSSubsystem::CreateIntegratedPlatform(EOS_Platform_Options& PlatformOptions)
{
	// Create the generic container.
	constexpr EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions CreateOptions =
	{
		EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST
	};

	const EOS_EResult Result = EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer(
		&CreateOptions, &PlatformOptions.IntegratedPlatformOptionsContainerHandle);
	if (Result != EOS_EResult::EOS_Success)
	{
		return Result;
	}

	// Configure platform-specific options.
	const EOS_IntegratedPlatform_Steam_Options PlatformSpecificOptions =
	{
		EOS_INTEGRATEDPLATFORM_STEAM_OPTIONS_API_LATEST,
		nullptr,
		1,
		48
	};

	// Add the configuration to the SDK initialization options.
	const EOS_IntegratedPlatform_Options Options =
	{
		EOS_INTEGRATEDPLATFORM_OPTIONS_API_LATEST,
		EOS_IPT_Steam,
		EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication |
		EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisableSDKManagedSessions,
		&PlatformSpecificOptions
	};

	const EOS_IntegratedPlatformOptionsContainer_AddOptions AddOptions =
	{
		EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST,
		&Options
	};

	return EOS_IntegratedPlatformOptionsContainer_Add(PlatformOptions.IntegratedPlatformOptionsContainerHandle,
	                                                  &AddOptions);
}

/**
 * Releases the temporary container created by 'CreateIntegratedPlatform'.
 *
 * @param Options The options that hold the container
 */
void UEOSSubsystem::FreeIntegratedPlatform(EOS_Platform_Options& PlatformOptions)
{
	// Free the container after SDK initialization.
	if (PlatformOptions.IntegratedPlatformOptionsContainerHandle)
	{
		EOS_IntegratedPlatformOptionsContainer_Release(PlatformOptions.IntegratedPlatformOptionsContainerHandle);
		PlatformOptions.IntegratedPlatformOptionsContainerHandle = nullptr;
	}
}


// -------------------------


/**
 * Callback used when the encrypted app ticket is ready.
 *
 * This is only called once. And can be used to login to the EOS platform when the game has just launched.
 */
void UEOSSubsystem::OnEncryptedAppTicketReady()
{
	UE_LOG(LogEOSSubsystem, Warning, TEXT("Encrypted App Ticket is ready!"));
	
	// Clear delegate, never called again.
	SteamManager->OnEncryptedAppTicketReady.Clear();
	LoginAuth();
}

/**
 * Gets an Encrypted App Ticket from Steam.
 *
 * Can be used as the token required for EOS_Connect_Login.
 */
std::string UEOSSubsystem::GetEncryptedAppTicketSteam()
{
	// TArray<uint8> AppTicketByteArray = SteamManager->RequestEncryptedAppTicket();
	// if (AppTicketByteArray.Num() <= 0)
	// {
	// 	UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to get encrypted app ticket from Steam"));
	// 	return "";
	// }
	//
	// char Buffer[1024] = "";
	// uint32_t Len = 1024;
	// EOS_EResult Result = EOS_ByteArray_ToString(AppTicketByteArray.GetData(), AppTicketByteArray.Num(), Buffer, &Len);
	//
	// if (Result != EOS_EResult::EOS_Success)
	// {
	// 	UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to convert encrypted app ticket to string"));
	// 	return "";
	// }
	//
	// return Buffer;
	return "";
}

/**
 * Gets an Auth Session Ticket from Steam.
 *
 * Can be used as the token required for both EOS_Connect_Login or EOS_Auth_Login.
 *
 * Preferred to use this for EOS_Auth_Login only, I believe you can't use this for both interfaces at the same time.
 */
std::string UEOSSubsystem::GetSessionTicketSteam()
{
	// TArray<uint8> AuthSessionTicketByteArray = SteamManager->GetSessionTicket();
	// if (AuthSessionTicketByteArray.Num() <= 0)
	// {
	// 	UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to get Auth Session Ticket from Steam"));
	// 	return "";
	// }
	//
	// char Buffer[1024] = "";
	// uint32_t Len = 1024;
	// EOS_EResult Result = EOS_ByteArray_ToString(AuthSessionTicketByteArray.GetData(), AuthSessionTicketByteArray.Num(), Buffer, &Len);
	//
	// if (Result != EOS_EResult::EOS_Success)
	// {
	// 	UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to convert encrypted app ticket to string"));
	// 	return "";
	// }
	//
	// return Buffer;
	return "";
}


// --------------------------------


/**
 * Login to the EOS Connect-Interface.
 *
 * THIS METHOD IS NOT COMPLETE, SHOULD HANDLE MULTIPLE PLATFORMS. NOW ONLY STEAM.
 */
void UEOSSubsystem::LoginConnect() {
	return;
	if(!EosPlatformHandle) return;
	// EOS_Platform_GetDesktopCrossplayStatus; // Implement later.
	
	// Currently steam, but should change to be able to login with other platforms.
	// GetEncryptedAppTicketPsn(); GetEncryptedAppTicketXbox(); GetEncryptedAppTicketSwitch(); etc. (If those platforms work the same).
	const std::string Token = GetEncryptedAppTicketSteam();
	// const std::string Token = GetAuthSessionTicketSteam();
	
	EOS_Connect_Credentials Credentials;
	Credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
	// Credentials.Type = EOS_EExternalCredentialType::EOS_ECT_STEAM_SESSION_TICKET; // Should also switch depending on platform.
	Credentials.Type = EOS_EExternalCredentialType::EOS_ECT_STEAM_APP_TICKET; // Should also switch depending on platform.
	Credentials.Token = Token.c_str();

	EOS_Connect_LoginOptions Options;
	Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
	Options.Credentials = &Credentials;
	Options.UserLoginInfo = nullptr;

	const EOS_HConnect ConnectHandle = EOS_Platform_GetConnectInterface(EosPlatformHandle);
	EOS_Connect_Login(ConnectHandle, &Options, this, OnLoginConnectComplete);
}

void EOS_CALL UEOSSubsystem::OnLoginConnectComplete(const EOS_Connect_LoginCallbackInfo* Data) {
	UE_LOG(LogEOSSubsystem, Log, TEXT("Login Connect-Interface Resultcode [%s]"), *FString(EOS_EResult_ToString(Data->ResultCode)));

	if(Data->ResultCode == EOS_EResult::EOS_Success)
	{
		UEOSSubsystem* EosSubsystemInstance = static_cast<UEOSSubsystem*>(Data->ClientData);
		if(!EosSubsystemInstance)
		{
			UE_LOG(LogEOSSubsystem, Error, TEXT("EOS-Subsystem not found in Login Connect-Interface callback"));
			return;
		}
		
		if(Data->LocalUserId)
		{
			UE_LOG(LogEOSSubsystem, Log, TEXT("Local-User-ID / Product-User-ID found"));
			EosSubsystemInstance->EosProductUserId = Data->LocalUserId;
		}
		else if(Data->ContinuanceToken)
		{
			// This should only be needed when there is not an Identity Provider present, like Steam, Epic, Playstation, etc.
			// Could be used in the editor.
			UE_LOG(LogEOSSubsystem, Log, TEXT("Continuance-Token found"));
			EosSubsystemInstance->EosContinuanceToken = Data->ContinuanceToken;
		}
		UE_LOG(LogEOSSubsystem, Log, TEXT("Login to EOS Connect-Interface successful"));
	}
	else
	{
		UE_LOG(LogEOSSubsystem, Error, TEXT("Login to EOS Connect-Interface failed"));
	}
}

void UEOSSubsystem::LogoutConnect()
{
}

void UEOSSubsystem::OnLogoutConnectComplete()
{
}


// --------------------------------


void UEOSSubsystem::LoginAuth()
{
	// if(!EosPlatformHandle) return;
	// UE_LOG(LogEOSSubsystem, Warning, TEXT("Login Auth-Interface"));
	//
	// SteamManager->OnSessionTicketReady.AddLambda([this]
	// {
	// 	SteamManager->OnSessionTicketReady.Clear();
	// 	
	// 	const std::string Token = GetSessionTicketSteam();
	// 	UE_LOG(LogEOSSubsystem, Log, TEXT("Token: %s"), *FString(Token.c_str()));
	// 	
	// 	EOS_Auth_Credentials Credentials;
	// 	Credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	// 	Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
	// 	Credentials.ExternalType = EOS_EExternalCredentialType::EOS_ECT_STEAM_SESSION_TICKET;
	// 	Credentials.Token = Token.c_str();
	//
	// 	EOS_Auth_LoginOptions Options;
	// 	Options.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	// 	Options.Credentials = &Credentials;
	// 	Options.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_Presence;
	//
	// 	const EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface(EosPlatformHandle);
	// 	if(!AuthHandle) return;
	// 	EOS_Auth_Login(AuthHandle, &Options, this, OnLoginAuthComplete);
	// });
	// SteamManager->RequestSessionTicketSteam();
}

void UEOSSubsystem::OnLoginAuthComplete(const EOS_Auth_LoginCallbackInfo* Data)
{
	UE_LOG(LogEOSSubsystem, Log, TEXT("Login Auth-Interface Resultcode [%s]"), *FString(EOS_EResult_ToString(Data->ResultCode)));

	if(Data->ResultCode == EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSSubsystem, Log, TEXT("Logged in to Auth-Interface"));
	}
	else
	{
		UE_LOG(LogEOSSubsystem, Error, TEXT("Failed to login to Auth-Interface"));
	}
}

void UEOSSubsystem::LogoutAuth()
{
}


// --------------------------------


// Could be changed to check if the user is logged-in.
// Maybe for when the user had no internet connection, but reconnects.
// And then wait for the login to complete before continuing with the session creation.
void UEOSSubsystem::CreateSession()
{
	EOS_Sessions_CreateSessionModificationOptions CreateSessionOptions = {};
	CreateSessionOptions.ApiVersion = EOS_SESSIONS_CREATESESSIONMODIFICATION_API_LATEST;
	CreateSessionOptions.SessionName = "MySession";
	CreateSessionOptions.BucketId = "GameMode:Region:MapName";
	CreateSessionOptions.MaxPlayers = 8;
	CreateSessionOptions.LocalUserId = EosProductUserId;
	CreateSessionOptions.bPresenceEnabled = true;
	CreateSessionOptions.bSanctionsEnabled = false;

	const EOS_HSessions SessionsHandle = EOS_Platform_GetSessionsInterface(EosPlatformHandle);
	EOS_HSessionModification SessionModification;
	
	const EOS_EResult Result = EOS_Sessions_CreateSessionModification(SessionsHandle, &CreateSessionOptions, &SessionModification);
	if (Result == EOS_EResult::EOS_Success)
	{
		EOS_Sessions_UpdateSessionOptions UpdateSessionOptions;
		UpdateSessionOptions.ApiVersion = EOS_SESSIONS_UPDATESESSION_API_LATEST;
		UpdateSessionOptions.SessionModificationHandle = SessionModification;
		
		EOS_Sessions_UpdateSession(SessionsHandle, &UpdateSessionOptions, nullptr, OnCreateSessionComplete);

		// Release the session modification handle
		EOS_SessionModification_Release(SessionModification);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session modification. Result code: [%s]"), *FString(EOS_EResult_ToString(Result)));
	}
}

void UEOSSubsystem::JoinSession()
{
}

void EOS_CALL UEOSSubsystem::OnCreateSessionComplete(const EOS_Sessions_UpdateSessionCallbackInfo* Data)
{
	UE_LOG(LogEOSSubsystem, Warning, TEXT("Session created: resultcode code: [%s]"), *FString(EOS_EResult_ToString(Data->ResultCode)));

	if(Data->ResultCode == EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSSubsystem, Log, TEXT("Session created successfully"));
	}
	else
	{
		UE_LOG(LogEOSSubsystem, Error, TEXT("Session creation failed"));
	}
}

void UEOSSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}