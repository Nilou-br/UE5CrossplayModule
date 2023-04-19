// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "PartyManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreatePartyComplete, bool);



UCLASS()
class MBGAME_API UPartyManager : public UObject
{
	GENERATED_BODY()

public:
	virtual void CreateParty();
	virtual void JoinParty();
	virtual void LeaveParty();
	virtual void SendInvite();
	
	FOnCreatePartyComplete OnCreatePartyComplete;

protected:
	class IOnlineSubsystem* Subsystem;
};