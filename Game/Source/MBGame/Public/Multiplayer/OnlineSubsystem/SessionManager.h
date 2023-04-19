// Copyright © 2023 Melvin Brink

#pragma once

#include "CoreMinimal.h"
#include "SessionManager.generated.h"




UCLASS()
class MBGAME_API USessionManager : public UObject
{
	GENERATED_BODY()

public:
	virtual void CreateSession();
	virtual void JoinSession();
	virtual void LeaveSession();
};