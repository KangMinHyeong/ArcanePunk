#pragma once
#include "CoreMinimal.h"

class APEditorErrorHelper
{
public:
    static void ReportErrorAndExitIfEditor(const FString& ErrorMessage, UObject* WorldContext = nullptr);
}; 