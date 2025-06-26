#include "Tools/APEditorErrorHelper.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Misc/MessageDialog.h"
#endif

/**
 * @brief 에디터에서 실행 중일 때, 심각한 오류가 발생했음을 알리고 PIE 세션만 종료하는 함수입니다.
 *
 * ✅ 동작 방식:
 * - 로그: 모든 빌드 환경에서 `UE_LOG`로 오류 메시지를 출력합니다.
 * - 에디터 환경 (WITH_EDITOR)에서만 추가적으로 다음을 수행합니다:
 *   1. 메시지 팝업을 띄워 사용자에게 오류를 즉시 알림 (`FMessageDialog`)
 *   2. PIE(Play In Editor) 세션을 종료 (`FGenericPlatformMisc::RequestExit(false)`)
 *      - 에디터 자체는 종료되지 않고 유지됩니다.
 *
 * ❗ Standalone 게임 실행 시에는 종료하지 않으며, 로그만 출력됩니다.
 *
 * 🔧 사용 예시:
 * - 데이터 테이블, 설정 값, 리소스가 유효하지 않은 경우
 * - 에디터 플러그인 도구나 테스트 시, 심각한 설정 오류가 감지된 경우
 * - PIE 상태에서만 작동하도록 설계된 오류 처리 루틴에 활용
 *
 * @param ErrorMessage  사용자에게 알릴 에러 메시지 (로그 및 팝업에 사용됨)
 * @param WorldContext  월드 정보 (현재는 사용하지 않음, 추후 확장성 고려해 남겨둠)
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