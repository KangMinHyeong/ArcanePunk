#include "Tools/APEditorErrorHelper.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Misc/MessageDialog.h"
#endif

void APEditorErrorHelper::ReportErrorAndExitIfEditor(const FString& ErrorMessage, UObject* WorldContext)
{
    UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);

#if WITH_EDITOR
    if (GIsEditor && !IsRunningCommandlet())
    {
        // 팝업 띄우기 (선택 사항)
        FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));

        // PIE 세션만 종료
        FGenericPlatformMisc::RequestExit(false);
    }
#endif
}