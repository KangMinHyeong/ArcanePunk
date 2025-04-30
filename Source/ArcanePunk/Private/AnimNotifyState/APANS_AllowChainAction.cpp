#include "AnimNotifyState/APANS_AllowChainAction.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogANS_ChainAction)

//@Defualt Setting
#pragma region Default Setting
UAPANS_AllowChainAction::UAPANS_AllowChainAction(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{}

void UAPANS_AllowChainAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    //@메시 컴포넌트 유효성 검사
    if (!MeshComp)
    {
        UE_LOGFMT(LogANS_ChainAction, Warning, "NotifyBegin 실패 - 유효하지 않은 메시 컴포넌트");
        return;
    }

    //@소유자 캐릭터 가져오기
    AArcanePunkCharacter* OwnerCharacter = Cast<AArcanePunkCharacter>(MeshComp->GetOwner());
    if (!OwnerCharacter)
    {
        UE_LOGFMT(LogANS_ChainAction, Warning, "NotifyBegin 실패 - 소유자가 ArcanePunkCharacter가 아님");
        return;
    }

    //@체인 액션 정보 유효성 검사
    if (!ChainActionInfo.EventTag.IsValid() || !ChainActionInfo.ActivationEventTag.IsValid())
    {
        UE_LOGFMT(LogANS_ChainAction, Warning, "NotifyBegin 실패 - 유효하지 않은 체인 액션 정보: 이벤트 태그({0}), 활성화 태그({1})",
            ChainActionInfo.EventTag.IsValid() ? *ChainActionInfo.EventTag.ToString() : TEXT("없음"),
            ChainActionInfo.ActivationEventTag.IsValid() ? *ChainActionInfo.ActivationEventTag.ToString() : TEXT("없음"));
        return;
    }

    //@체인 윈도우 시작
    UE_LOGFMT(LogANS_ChainAction, Log, "체인 윈도우 시작 - 애니메이션: {0}, 이벤트 태그: {1}, 활성화 태그: {2}, 모드: {3}",
        *Animation->GetName(),
        *ChainActionInfo.EventTag.ToString(),
        *ChainActionInfo.ActivationEventTag.ToString(),
        ChainActionInfo.ChainActionMode == EChainActionMode::Delayed ? TEXT("지연 실행") : TEXT("즉시 실행"));

    OwnerCharacter->StartChainWindow(ChainActionInfo);
}

void UAPANS_AllowChainAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //@메시 컴포넌트 유효성 검사
    if (!MeshComp)
    {
        UE_LOGFMT(LogANS_ChainAction, Warning, "NotifyEnd 실패 - 유효하지 않은 메시 컴포넌트");
        return;
    }

    //@소유자 캐릭터 가져오기
    AArcanePunkCharacter* OwnerCharacter = Cast<AArcanePunkCharacter>(MeshComp->GetOwner());
    if (!OwnerCharacter)
    {
        UE_LOGFMT(LogANS_ChainAction, Warning, "NotifyEnd 실패 - 소유자가 ArcanePunkCharacter가 아님");
        return;
    }

    //@체인 윈도우가 활성화되어 있는지 확인
    if (!OwnerCharacter->IsChainWindowActive())
    {
        UE_LOGFMT(LogANS_ChainAction, Log, "NotifyEnd - 체인 윈도우가 이미 비활성화되어 있음");
        return;
    }

    //@체인 윈도우 종료
    UE_LOGFMT(LogANS_ChainAction, Log, "체인 윈도우 종료 - 애니메이션: {0}", *Animation->GetName());
    OwnerCharacter->EndChainWindow();
}
#pragma endregion

//@Property/Info...etc
#pragma region Property or Subwidgets or Infos...etc
#pragma endregion

//@Delegates
#pragma region Delegates
#pragma endregion

//@Callbacks
#pragma region Callbacks
#pragma endregion

//@Utility(Setter, Getter,...etc)
#pragma region Utility
#pragma endregion
