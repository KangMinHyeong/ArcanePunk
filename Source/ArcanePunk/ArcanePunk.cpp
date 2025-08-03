// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcanePunk.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "Features/IModularFeatures.h"
#include "Tools/ArcanePunkPIEAuthorizer.h"
#endif

class FArcanePunkGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
#if WITH_EDITOR
		PIEAuthorizer.Emplace();
		IModularFeatures::Get().RegisterModularFeature(IPIEAuthorizer::GetModularFeatureName(), &PIEAuthorizer.GetValue());
#endif
	}

	virtual void ShutdownModule() override
	{
#if WITH_EDITOR
		if (PIEAuthorizer.IsSet())
		{
			IModularFeatures::Get().UnregisterModularFeature(IPIEAuthorizer::GetModularFeatureName(), &PIEAuthorizer.GetValue());
			PIEAuthorizer.Reset();
		}
#endif
	}

private:
#if WITH_EDITOR
	TOptional<FArcanePunkPIEAuthorizer> PIEAuthorizer;
#endif
};

IMPLEMENT_PRIMARY_GAME_MODULE(FArcanePunkGameModule, ArcanePunk, "ArcanePunk");
