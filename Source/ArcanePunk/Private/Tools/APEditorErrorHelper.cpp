#include "Tools/APEditorErrorHelper.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Misc/MessageDialog.h"
#endif

/**
 * @brief 에디터 환경에서 심각한 오류가 발생했을 때 PIE 세션만 종료하는 유틸리티 함수입니다.
 *
 * ✅ 동작 방식:
 * - 모든 환경에서 오류 메시지를 로그로 출력합니다.
 * - 에디터 환경(WITH_EDITOR)에서만 다음을 수행합니다:
 *   1. 오류 메시지를 팝업(Alert)으로 표시합니다.
 *   2. PIE(Play In Editor) 세션만 종료하고, 에디터 자체는 유지합니다.
 *
 * ❗ 스탠드얼론 게임에서는 종료되지 않고 로그만 출력됩니다.
 *
 * 🔧 사용 예시:
 * - PIE 상태에서 잘못된 설정, 누락된 리소스, 유효하지 않은 데이터가 감지되었을 때
 * - 복구 불가능한 에디터 플러그인 오류가 발생했을 때
 *
 * @param ErrorMessage 사용자에게 표시할 에러 메시지입니다.
 */

void APEditorErrorHelper::ReportErrorAndExitIfEditor(const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);

#if WITH_EDITOR
    if (GIsEditor && !IsRunningCommandlet())
    {
        FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));
        FGenericPlatformMisc::RequestExit(false); // PIE 세션만 종료
    }
#endif
}