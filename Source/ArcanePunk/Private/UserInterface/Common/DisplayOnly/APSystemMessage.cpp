
#include "UserInterface/Common/DisplayOnly/APSystemMessage.h"
#include "Logging/StructuredLog.h"

#include "GameInstance/APGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

#include "Kismet/GameplayStatics.h"
#include "GameInstance/Subsystem/APSystemMessageSubsystem.h"

#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanelSlot.h"

DEFINE_LOG_CATEGORY(LogSystemMessageUI)

#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Misc/MessageDialog.h"
#include "Engine/Engine.h"
#endif

bool USystemMessageConfig::ValidateConfiguration(FString& OutErrorMessage) const
{
    OutErrorMessage.Empty();

    // 1단계: Min/Max 값 검증 (기존 검증)
    if (!ValidateMinMaxValues(OutErrorMessage))
    {
        return false;
    }

    // 2단계: 0값 검증 (기존 검증)
    if (!ValidateNonZeroValues(OutErrorMessage))
    {
        return false;
    }

    // 3단계: 새로운 검증 - Fade 시간 동일성 검사
    // 이 검증은 사용자 경험의 일관성을 위해 매우 중요합니다
    // 애니메이션의 대칭성은 자연스러운 UI 흐름을 만드는 핵심 요소입니다
    if (!ValidateFadeTimesEquality(OutErrorMessage))
    {
        return false;
    }

    // 4단계: 새로운 검증 - 폰트 크기 범위 검사
    // UI 일관성과 가독성을 보장하기 위한 필수적인 검증입니다
    // 12-24픽셀 범위는 게임 UI에서 최적의 가독성을 제공하는 검증된 범위입니다
    if (!ValidateFontSizeRange(OutErrorMessage))
    {
        return false;
    }

    // 모든 검증을 통과한 경우
    OutErrorMessage = TEXT("모든 설정값이 유효합니다.");
    return true;
}

#if WITH_EDITOR

void USystemMessageConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // 변경된 프로퍼티가 있을 때만 검증 수행
    if (PropertyChangedEvent.Property != nullptr)
    {
        const FName PropertyName = PropertyChangedEvent.Property->GetFName();

        // 폰트 크기가 변경되었을 때 - 새로운 검증 규칙
        // 폰트 크기는 UI의 핵심 요소이므로 즉시 범위 검사와 수정이 필요합니다
        if (PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, FontSize))
        {
            FixFontSizeRange();

            // 폰트 크기 변경은 다른 크기 계산에도 영향을 미치므로 추가 검증 수행
            FixZeroValues();
            FixMinMaxValues();
        }

        // Fade 시간이 변경되었을 때 - 새로운 검증 규칙
        // FadeIn과 FadeOut 시간의 동일성은 애니메이션 일관성을 위해 매우 중요합니다
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, FadeInTime) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, FadeOutTime))
        {
            // 먼저 0값을 수정한 다음, Fade 시간 동일성을 검사합니다
            // 이 순서는 매우 중요합니다 - 0값이 있으면 동일성 검사가 의미없어지기 때문입니다
            FixZeroValues();
            FixFadeTimesEquality();
        }

        // Scale 관련 프로퍼티가 변경되었을 때 (기존 로직 유지)
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, MinWidthScale) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, MaxWidthScale) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, MinHeightScale) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, MaxHeightScale))
        {
            // 0값과 Min/Max 관계 자동 수정
            FixZeroValues();
            FixMinMaxValues();
        }

        // 기타 시간 관련 프로퍼티가 변경되었을 때 (기존 로직 유지하되 확장)
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, LifeSpan) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, MaxRowSize) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(USystemMessageConfig, MaxColumnSize))
        {
            // 0값 자동 수정
            FixZeroValues();
        }

        // 최종 전체 유효성 검사 수행 및 결과 표시
        // 이는 모든 개별 수정 작업이 완료된 후 전체적인 일관성을 확인하는 단계입니다
        FString ValidationMessage;
        bool bIsValid = ValidateConfiguration(ValidationMessage);

        if (!bIsValid)
        {
            // 자동 수정으로도 해결되지 않은 문제가 있는 경우
            UE_LOG(LogTemp, Error, TEXT("SystemMessageConfig 유효성 검사 실패: %s"), *ValidationMessage);
            ShowEditorNotification(ValidationMessage, true);
        }
        else
        {
            // 모든 검증을 통과한 경우 성공 메시지 (선택적)
            // Verbose 레벨로 설정하여 개발 중에만 보이도록 합니다
            UE_LOG(LogTemp, Verbose, TEXT("SystemMessageConfig 검증 성공: %s"), *ValidationMessage);
        }
    }
}

void USystemMessageConfig::PostInitProperties()
{
    Super::PostInitProperties();

    // 초기화 시에도 유효성 검사 수행
#if WITH_EDITOR
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        FixZeroValues();
        FixMinMaxValues();
    }
#endif
}

bool USystemMessageConfig::ValidateFadeTimesEquality(FString& OutErrorMessage) const
{
    // Fade-In과 Fade-Out 시간이 동일한지 검사합니다
    // 이 검증은 애니메이션의 대칭성과 사용자 경험의 일관성을 보장하기 위한 것입니다

    if (FadeInTime != FadeOutTime)
    {
        OutErrorMessage = FString::Printf(
            TEXT("FadeInTime(%d초)과 FadeOutTime(%d초)이 동일하지 않습니다. 애니메이션 일관성을 위해 두 값은 같아야 합니다."),
            FadeInTime, FadeOutTime
        );
        return false;
    }

    return true;
}

bool USystemMessageConfig::ValidateFontSizeRange(FString& OutErrorMessage) const
{
    // 폰트 크기가 허용된 범위(12-24) 내에 있는지 검사합니다
    // 이 범위는 게임 UI에서 최적의 가독성을 제공하는 검증된 범위입니다

    const int32 MinAllowedFontSize = 12;
    const int32 MaxAllowedFontSize = 24;

    if (FontSize < MinAllowedFontSize)
    {
        OutErrorMessage = FString::Printf(
            TEXT("FontSize(%d)가 최소 허용값(%d)보다 작습니다. 가독성 확보를 위해 최소 %d 이상이어야 합니다."),
            FontSize, MinAllowedFontSize, MinAllowedFontSize
        );
        return false;
    }

    if (FontSize > MaxAllowedFontSize)
    {
        OutErrorMessage = FString::Printf(
            TEXT("FontSize(%d)가 최대 허용값(%d)보다 큽니다. UI 일관성을 위해 최대 %d 이하여야 합니다."),
            FontSize, MaxAllowedFontSize, MaxAllowedFontSize
        );
        return false;
    }

    return true;
}

void USystemMessageConfig::FixFadeTimesEquality()
{
    // Fade 시간이 다른 경우 자동으로 동일하게 맞춥니다
    // 기본적으로 FadeInTime을 기준으로 FadeOutTime을 맞춥니다
    // 이 선택은 "입력 우선순위" 원칙에 따른 것입니다 - 먼저 설정된 값을 존중합니다

    if (FadeInTime != FadeOutTime)
    {
        int32 OriginalFadeOutTime = FadeOutTime;

        // FadeInTime이 더 최근에 변경되었을 가능성이 높으므로 이를 기준값으로 사용합니다
        // 하지만 실제로는 어느 것이 먼저 변경되었는지 알기 어려우므로,
        // 더 안전한 값(0이 아닌 값)을 선택하는 로직을 구현합니다

        if (FadeInTime > 0 && FadeOutTime <= 0)
        {
            // FadeInTime만 유효한 경우
            FadeOutTime = FadeInTime;
        }
        else if (FadeOutTime > 0 && FadeInTime <= 0)
        {
            // FadeOutTime만 유효한 경우
            FadeInTime = FadeOutTime;
        }
        else if (FadeInTime > 0 && FadeOutTime > 0)
        {
            // 둘 다 유효한 경우, FadeInTime을 기준으로 설정
            // 이는 UI 디자인에서 "입장 애니메이션 우선" 원칙에 따른 것입니다
            FadeOutTime = FadeInTime;
        }
        else
        {
            // 둘 다 유효하지 않은 경우, 기본값으로 설정
            FadeInTime = 1;
            FadeOutTime = 1;
        }

        UE_LOG(LogTemp, Warning,
            TEXT("Fade 시간 불일치로 인한 자동 수정 - FadeInTime: %d초, FadeOutTime: %d->%d초"),
            FadeInTime, OriginalFadeOutTime, FadeOutTime
        );

        // 사용자에게 친화적인 알림 제공
        ShowEditorNotification(
            FString::Printf(TEXT("Fade 시간이 동일하지 않아 자동으로 맞췄습니다. (FadeIn/Out: %d초)"), FadeInTime),
            false  // 경고가 아닌 정보성 알림
        );
    }
}

void USystemMessageConfig::FixFontSizeRange()
{
    // 폰트 크기가 허용 범위를 벗어난 경우 자동으로 수정합니다
    // 이 함수는 "클램핑(Clamping)" 기법을 사용하여 값을 유효한 범위로 제한합니다

    const int32 MinAllowedFontSize = 12;
    const int32 MaxAllowedFontSize = 24;
    int32 OriginalFontSize = FontSize;

    // 범위를 벗어난 값을 허용 범위로 클램핑
    FontSize = FMath::Clamp(FontSize, MinAllowedFontSize, MaxAllowedFontSize);

    // 값이 실제로 변경되었는지 확인하고 적절한 로그와 알림 제공
    if (OriginalFontSize != FontSize)
    {
        FString ChangeDescription;
        bool bWasTooSmall = OriginalFontSize < MinAllowedFontSize;

        if (bWasTooSmall)
        {
            ChangeDescription = FString::Printf(
                TEXT("FontSize가 너무 작아서(%d) 최소값(%d)으로 자동 조정되었습니다."),
                OriginalFontSize, FontSize
            );
        }
        else
        {
            ChangeDescription = FString::Printf(
                TEXT("FontSize가 너무 커서(%d) 최대값(%d)으로 자동 조정되었습니다."),
                OriginalFontSize, FontSize
            );
        }

        UE_LOG(LogTemp, Warning, TEXT("SystemMessageConfig: %s"), *ChangeDescription);

        // 사용자에게 명확한 피드백 제공
        ShowEditorNotification(ChangeDescription, true);  // 이것은 제약 위반이므로 경고로 표시
    }
}

void USystemMessageConfig::ShowEditorNotification(const FString& Message, bool bIsError) const
{
    // Unreal Engine의 에디터 알림 시스템을 사용하여 사용자에게 즉각적인 피드백을 제공합니다
    // 이는 "즉시 피드백" 원칙을 구현하는 현대적인 UI 패턴입니다

    FNotificationInfo Info(FText::FromString(Message));

    // 알림의 생명주기 설정 - 사용자가 읽을 충분한 시간을 제공하되 너무 오래 남지 않도록 합니다
    Info.bFireAndForget = true;          // 자동으로 사라지는 알림
    Info.FadeOutDuration = 2.0f;         // 2초에 걸쳐 서서히 사라짐
    Info.ExpireDuration = 5.0f;          // 총 5초간 표시 (읽기에 충분한 시간)

    // 메시지 유형에 따른 시각적 구분 - 사용자가 한눈에 중요도를 파악할 수 있게 합니다
    // Unreal Engine 버전에 따른 호환성 처리
    const FSlateBrush* IconBrush = nullptr;

#if ENGINE_MAJOR_VERSION >= 5
    // UE5 이상: 현대적인 FAppStyle 사용
    if (bIsError)
    {
        IconBrush = FAppStyle::GetBrush(TEXT("MessageLog.Error"));
    }
    else
    {
        IconBrush = FAppStyle::GetBrush(TEXT("MessageLog.Info"));
    }
#else
    // UE4: 레거시 FEditorStyle 사용
    if (bIsError)
    {
        IconBrush = FEditorStyle::GetBrush(TEXT("MessageLog.Error"));
    }
    else
    {
        IconBrush = FEditorStyle::GetBrush(TEXT("MessageLog.Info"));
    }
#endif

    // 아이콘이 성공적으로 로드되었는지 확인 (방어적 프로그래밍)
    if (IconBrush)
    {
        Info.Image = IconBrush;
    }
    else
    {
        // 폴백: 아이콘 로드 실패 시에도 알림은 표시되도록 합니다
        UE_LOG(LogTemp, Verbose, TEXT("SystemMessageConfig: 알림 아이콘을 로드할 수 없습니다."));
    }

    // 알림을 실제로 화면에 표시합니다
    FSlateNotificationManager::Get().AddNotification(Info);

    // 로그에도 기록하여 나중에 추적할 수 있게 합니다
    // 이는 디버깅과 이슈 트래킹에 매우 유용합니다
    if (bIsError)
    {
        UE_LOG(LogTemp, Warning, TEXT("SystemMessageConfig 경고: %s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("SystemMessageConfig 정보: %s"), *Message);
    }
}

bool USystemMessageConfig::ValidateMinMaxValues(FString& OutErrorMessage) const
{
    // Width Scale 검증
    if (MinWidthScale >= MaxWidthScale)
    {
        OutErrorMessage = FString::Printf(
            TEXT("MinWidthScale(%.2f)은 MaxWidthScale(%.2f)보다 작아야 합니다."),
            MinWidthScale, MaxWidthScale
        );
        return false;
    }

    // Height Scale 검증
    if (MinHeightScale >= MaxHeightScale)
    {
        OutErrorMessage = FString::Printf(
            TEXT("MinHeightScale(%.2f)은 MaxHeightScale(%.2f)보다 작아야 합니다."),
            MinHeightScale, MaxHeightScale
        );
        return false;
    }

    return true;
}

bool USystemMessageConfig::ValidateNonZeroValues(FString& OutErrorMessage) const
{
    // 정수 값들 검증
    if (MaxRowSize <= 0)
    {
        OutErrorMessage = TEXT("MaxRowSize는 0보다 커야 합니다.");
        return false;
    }

    if (MaxColumnSize <= 0)
    {
        OutErrorMessage = TEXT("MaxColumnSize는 0보다 커야 합니다.");
        return false;
    }

    if (LifeSpan <= 0)
    {
        OutErrorMessage = TEXT("LifeSpan은 0보다 커야 합니다.");
        return false;
    }

    if (FadeInTime <= 0)
    {
        OutErrorMessage = TEXT("FadeInTime은 0보다 커야 합니다.");
        return false;
    }

    if (FadeOutTime <= 0)
    {
        OutErrorMessage = TEXT("FadeOutTime은 0보다 커야 합니다.");
        return false;
    }

    // float 값들 검증
    if (MinWidthScale <= 0.0f)
    {
        OutErrorMessage = TEXT("MinWidthScale은 0보다 커야 합니다.");
        return false;
    }

    if (MaxWidthScale <= 0.0f)
    {
        OutErrorMessage = TEXT("MaxWidthScale은 0보다 커야 합니다.");
        return false;
    }

    if (MinHeightScale <= 0.0f)
    {
        OutErrorMessage = TEXT("MinHeightScale은 0보다 커야 합니다.");
        return false;
    }

    if (MaxHeightScale <= 0.0f)
    {
        OutErrorMessage = TEXT("MaxHeightScale은 0보다 커야 합니다.");
        return false;
    }

    return true;
}

void USystemMessageConfig::FixMinMaxValues()
{
    // Width Scale 자동 수정
    if (MinWidthScale >= MaxWidthScale)
    {
        // Min이 Max보다 크거나 같으면, Max를 Min의 1.5배로 설정
        MaxWidthScale = MinWidthScale * 1.5f;

        UE_LOG(LogTemp, Warning,
            TEXT("MinWidthScale이 MaxWidthScale보다 크거나 같아서 MaxWidthScale을 %.2f로 자동 조정했습니다."),
            MaxWidthScale
        );
    }

    // Height Scale 자동 수정
    if (MinHeightScale >= MaxHeightScale)
    {
        // Min이 Max보다 크거나 같으면, Max를 Min의 1.5배로 설정
        MaxHeightScale = MinHeightScale * 1.5f;

        UE_LOG(LogTemp, Warning,
            TEXT("MinHeightScale이 MaxHeightScale보다 크거나 같아서 MaxHeightScale을 %.2f로 자동 조정했습니다."),
            MaxHeightScale
        );
    }
}

void USystemMessageConfig::FixZeroValues()
{
    // 정수 값들 자동 수정
    if (MaxRowSize <= 0)
    {
        MaxRowSize = 1;
        UE_LOG(LogTemp, Warning, TEXT("MaxRowSize가 0 이하여서 1로 자동 조정했습니다."));
    }

    if (MaxColumnSize <= 0)
    {
        MaxColumnSize = 1;
        UE_LOG(LogTemp, Warning, TEXT("MaxColumnSize가 0 이하여서 1로 자동 조정했습니다."));
    }

    if (LifeSpan <= 0)
    {
        LifeSpan = 1;
        UE_LOG(LogTemp, Warning, TEXT("LifeSpan이 0 이하여서 1로 자동 조정했습니다."));
    }

    if (FadeInTime <= 0)
    {
        FadeInTime = 1;
        UE_LOG(LogTemp, Warning, TEXT("FadeInTime이 0 이하여서 1로 자동 조정했습니다."));
    }

    if (FadeOutTime <= 0)
    {
        FadeOutTime = 1;
        UE_LOG(LogTemp, Warning, TEXT("FadeOutTime이 0 이하여서 1로 자동 조정했습니다."));
    }

    // float 값들 자동 수정
    const float MinAllowedScale = 0.1f;

    if (MinWidthScale <= 0.0f)
    {
        MinWidthScale = MinAllowedScale;
        UE_LOG(LogTemp, Warning, TEXT("MinWidthScale이 0 이하여서 %.1f로 자동 조정했습니다."), MinAllowedScale);
    }

    if (MaxWidthScale <= 0.0f)
    {
        MaxWidthScale = MinAllowedScale;
        UE_LOG(LogTemp, Warning, TEXT("MaxWidthScale이 0 이하여서 %.1f로 자동 조정했습니다."), MinAllowedScale);
    }

    if (MinHeightScale <= 0.0f)
    {
        MinHeightScale = MinAllowedScale;
        UE_LOG(LogTemp, Warning, TEXT("MinHeightScale이 0 이하여서 %.1f로 자동 조정했습니다."), MinAllowedScale);
    }

    if (MaxHeightScale <= 0.0f)
    {
        MaxHeightScale = MinAllowedScale;
        UE_LOG(LogTemp, Warning, TEXT("MaxHeightScale이 0 이하여서 %.1f로 자동 조정했습니다."), MinAllowedScale);
    }
}

#endif // WITH_EDITOR

//@Defualt Setting
#pragma region Default Setting
void UAPSystemMessage::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    //@System Message Config 로드
    LoadSystemMessageConfig();

    //@시스템 메시지 서브시스템 이벤트 구독
    auto GameInsatnce = UGameplayStatics::GetGameInstance(GetWorld());
    if (!GameInsatnce)
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "게임 인스턴스를 찾을 수 없음");
        return;
    }

    //@System Message Subsystem
    if (auto* SystemMessageSubsystem = GameInsatnce->GetSubsystem<UAPSystemMessageSubsystem>())
    {
        SystemMessageSubsystem->RequestShowSystemMessage.BindUFunction(this, "OnRequestShowSystemMessage");
        UE_LOGFMT(LogSystemMessageUI, Log, "시스템 메시지 서브시스템 이벤트 구독 완료");
    }
    else
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "시스템 메시지 서브시스템을 찾을 수 없음");
    }
}
#pragma endregion

//@Property/Info...etc
#pragma region Property or Subwidgets or Infos...etc
void UAPSystemMessage::LoadSystemMessageConfig()
{
    if (CachedMessageConfig)
    {
        return;
    }

    CachedMessageConfig = LoadObject<USystemMessageConfig>(nullptr, TEXT("/Game/DataTable/ui/DA_SystemMessage_Config"));

    if (!CachedMessageConfig)
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "시스템 메시지 설정 데이터 애셋 로딩 실패");
        return;
    }

    UE_LOGFMT(LogSystemMessageUI, Log, "시스템 메시지 설정 데이터 애셋 로딩 성공");

    //@Config 설정 적용
    ApplyConfigSettings();
}

void UAPSystemMessage::ApplyConfigSettings()
{
    if (!CachedMessageConfig)
    {
        UE_LOGFMT(LogSystemMessageUI, Warning, "Config가 로드되지 않아 설정 적용 불가");
        return;
    }

    // 폰트 크기 설정
    if (Text_High)
    {
        FSlateFontInfo FontInfo = Text_High->GetFont();
        FontInfo.Size = CachedMessageConfig->GetFontSize();
        Text_High->SetFont(FontInfo);
    }

    // Content_High 초기 크기를 최소 크기로 설정
    if (Content_High)
    {
        Content_High->SetWidthOverride(CachedMessageConfig->GetMinWidth());
        Content_High->SetHeightOverride(CachedMessageConfig->GetMinHeight());
    }

    UE_LOGFMT(LogSystemMessageUI, Log,
        "Config 적용 완료 - 폰트: {0}pt, 최소크기: {1}x{2}, 최대크기: {3}x{4}",
        CachedMessageConfig->GetFontSize(),
        CachedMessageConfig->GetMinWidth(),
        CachedMessageConfig->GetMinHeight(),
        CachedMessageConfig->GetMaxWidth(),
        CachedMessageConfig->GetMaxHeight());
}

void UAPSystemMessage::UpdateFadeOpacity()
{
    // 안전성 검사 - 필수 컴포넌트들이 준비되어 있는지 확인
    if (!Content_High || !bIsCurrentlyAnimating)
    {
        return;
    }

    float CurrentGameTime = GetWorld()->GetTimeSeconds();
    float ElapsedTime = CurrentGameTime - AnimationStartTime;

    float AnimationProgress = ElapsedTime / AnimationDuration;
    AnimationProgress = FMath::Clamp(AnimationProgress, 0.0f, 1.0f);
    CurrentOpacity = FMath::Lerp(StartOpacity, EndOpacity, AnimationProgress);

    // 5단계: 계산된 값을 UI에 실제로 적용
    Content_High->SetRenderOpacity(CurrentOpacity);

    // 6단계: 애니메이션 완료 검사 및 다음 단계 진행
    if (AnimationProgress >= 1.0f)
    {
        // 애니메이션이 정확히 완료되었습니다
        CompleteCurrentAnimation();
    }

}

void UAPSystemMessage::StartFadeIn()
{
    // 안전성 검사 - 필수 요소들이 준비되었는지 확인
    if (!CachedMessageConfig || !Content_High)
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "Fade-In 시작 실패: 필수 컴포넌트 누락");
        return;
    }

    // 1단계: 애니메이션 매개변수 설정
    // Config에서 정확한 시간 값을 가져와서 float로 변환합니다
    AnimationDuration = static_cast<float>(CachedMessageConfig->GetFadeInTime());
    AnimationStartTime = GetWorld()->GetTimeSeconds();  // 현재 시간을 시작점으로 기록

    // 2단계: 투명도 범위 설정 (완전 투명에서 완전 불투명으로)
    StartOpacity = -1.0f;
    EndOpacity = 1.0f;
    CurrentOpacity = StartOpacity;  // 시작값으로 즉시 설정

    // 3단계: 애니메이션 상태 활성화
    bIsCurrentlyAnimating = true;

    // 4단계: UI 초기 상태 설정
    Content_High->SetRenderOpacity(CurrentOpacity);
    SetVisibility(ESlateVisibility::Visible);  // UI를 화면에 표시

    // 5단계: 업데이트 타이머 시작
    // 60fps로 부드러운 애니메이션을 제공하되, 실제 시간 계산은 절대 시간 기준
    GetWorld()->GetTimerManager().SetTimer(
        FadeTimerHandle,
        this,
        &UAPSystemMessage::UpdateFadeOpacity,
        1.0f / 60.0f,  // 60fps - 부드러운 시각적 효과를 위한 빈번한 업데이트
        true           // 반복 실행
    );
}

void UAPSystemMessage::StartFadeOut()
{
    // 안전성 검사
    if (!CachedMessageConfig || !Content_High)
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "Fade-Out 시작 실패: 필수 컴포넌트 누락");
        return;
    }

    // 1단계: 애니메이션 매개변수 설정
    AnimationDuration = static_cast<float>(CachedMessageConfig->GetFadeOutTime());
    AnimationStartTime = GetWorld()->GetTimeSeconds();

    // 2단계: 투명도 범위 설정 (완전 불투명에서 완전 투명으로)
    StartOpacity = 1.0f;
    EndOpacity = 0.0f;
    CurrentOpacity = StartOpacity;  // 현재 상태를 시작값으로 설정

    // 3단계: 애니메이션 상태 활성화
    bIsCurrentlyAnimating = true;

    // 4단계: 업데이트 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(
        FadeTimerHandle,
        this,
        &UAPSystemMessage::UpdateFadeOpacity,
        1.0f / 60.0f,
        true
    );
}

void UAPSystemMessage::StartLifeSpan()
{
    if (!CachedMessageConfig)
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "LifeSpan 시작 실패: Config 누락");
        return;
    }

    // 애니메이션이 완료되었으므로 상태를 안정화합니다
    bIsCurrentlyAnimating = false;
    CurrentOpacity = 1.0f;
    Content_High->SetRenderOpacity(1.0f);

    // 모든 기존 타이머를 정리합니다
    GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

    // LifeSpan 타이머 설정 - 정확한 지속 시간 사용
    float LifeSpanDuration = static_cast<float>(CachedMessageConfig->GetLifeSpan());

    GetWorld()->GetTimerManager().SetTimer(
        LifeSpanTimerHandle,
        this,
        &UAPSystemMessage::StartFadeOut,
        LifeSpanDuration,
        false  // 한 번만 실행
    );

    UE_LOGFMT(LogSystemMessageUI, Log,
        "LifeSpan 시작 - 지속시간: {0}초", LifeSpanDuration);
}

void UAPSystemMessage::CompleteCurrentAnimation()
{
    // 1단계: 애니메이션 상태 정리
    bIsCurrentlyAnimating = false;
    GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

    // 2단계: 정확한 최종값으로 설정 (부동소수점 오차 제거)
    CurrentOpacity = EndOpacity;
    Content_High->SetRenderOpacity(CurrentOpacity);

    // 3단계: 다음 단계 결정 및 진행
    if (FMath::IsNearlyEqual(EndOpacity, 1.0f, 0.01f))
    {
        // Fade-In이 완료된 경우 - LifeSpan 단계로 진행
        UE_LOGFMT(LogSystemMessageUI, Log, "Fade-In 완료 - LifeSpan 시작");
        StartLifeSpan();
    }
    else
    {
        // Fade-Out이 완료된 경우 - 메시지 숨김 처리
        UE_LOGFMT(LogSystemMessageUI, Log, "Fade-Out 완료 - 메시지 숨김");
        HideSystemMessage();
    }
}

void UAPSystemMessage::DisplaySystemMesseage(const FString& SystemMessageText)
{
    if (!Text_High || !Content_High || !CachedMessageConfig)
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "필수 컴포넌트가 초기화되지 않음");
        return;
    }

    if (bIsActive)
    {
        bIsBlocked = true;
        UE_LOGFMT(LogSystemMessageUI, Warning, "시스템 메시지가 이미 활성화됨 - 요청 차단");
        return;
    }

    bIsActive = true;
    bIsBlocked = false;
    SystemMessageActivated.Broadcast();

    // FText 처리 및 개행
    FString ProcessedText = ProcessTextForDisplay(SystemMessageText);
    Text_High->SetText(FText::FromString(ProcessedText));

    Content_High->SetVisibility(ESlateVisibility::Visible);
    StartFadeIn();

    UE_LOGFMT(LogSystemMessageUI, Log, "시스템 메시지 시작: {0}", *SystemMessageText);
}

// ProcessTextForDisplay 함수 수정 - \n만 처리
FString UAPSystemMessage::ProcessTextForDisplay(const FString& InputText)
{
    if (!CachedMessageConfig) return InputText;

    const int32 MaxRowSize = CachedMessageConfig->GetMaxRowSize();
    const int32 MaxColumnSize = CachedMessageConfig->GetMaxColumnSize();

    // \n을 실제 개행으로 변환
    FString ProcessedInput = InputText.Replace(TEXT("\\n"), TEXT("\n"));

    TArray<FString> Lines;
    FString CurrentLine;

    // 문자별 처리
    for (int32 i = 0; i < ProcessedInput.Len(); ++i)
    {
        TCHAR CurrentChar = ProcessedInput[i];

        // 명시적 개행 처리
        if (CurrentChar == TEXT('\n'))
        {
            Lines.Add(CurrentLine);
            CurrentLine.Empty();
            continue;
        }

        // 현재 줄에 문자 추가
        CurrentLine += CurrentChar;

        // 자동 개행 조건 체크 (30자 또는 MaxRowSize 도달)
        if (CurrentLine.Len() >= 30 || CurrentLine.Len() >= MaxRowSize)
        {
            Lines.Add(CurrentLine);
            CurrentLine.Empty();
        }
    }

    // 마지막 줄 추가
    if (!CurrentLine.IsEmpty())
    {
        Lines.Add(CurrentLine);
    }

    // 최대 줄 수 제한
    if (Lines.Num() > MaxColumnSize)
    {
        Lines.SetNum(MaxColumnSize);
    }

    // Content_High 크기 업데이트
    UpdateContentHighSize(Lines);

    // 최종 텍스트 생성
    return FString::Join(Lines, TEXT("\n"));
}

void UAPSystemMessage::UpdateContentHighSize(const TArray<FString>& Lines)
{
    if (!Content_High || !CachedMessageConfig) return;

    // 한글 고려 최대 표시 길이 계산
    int32 MaxDisplayLength = 0;
    for (const FString& Line : Lines)
    {
        int32 DisplayLength = 0;
        for (int32 i = 0; i < Line.Len(); ++i)
        {
            TCHAR Char = Line[i];
            if ((Char >= 0x1100 && Char <= 0xD7AF) || // 한글 범위
                (Char >= 0x4E00 && Char <= 0x9FFF))   // 한자 범위
            {
                DisplayLength += 2;
            }
            else
            {
                DisplayLength += 1;
            }
        }
        MaxDisplayLength = FMath::Max(MaxDisplayLength, DisplayLength);
    }

    float Width = CachedMessageConfig->CalculateWidth(MaxDisplayLength);
    float Height = CachedMessageConfig->CalculateHeight(Lines.Num());

    if (UPanelSlot* ParentSlot = Content_High->Slot)
    {
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ParentSlot))
        {
            CanvasSlot->SetSize(FVector2D(Width, Height));
        }
    }

    UE_LOGFMT(LogSystemMessageUI, Log,
        "크기 계산 - 표시길이: {0}, 줄수: {1}, 크기: {2}x{3}",
        MaxDisplayLength, Lines.Num(), Width, Height);
}

void UAPSystemMessage::HideSystemMessage()
{
    SetVisibility(ESlateVisibility::Collapsed);
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

    //@생명주기 종료
    bIsActive = false;
    SystemMessageDeactivated.Broadcast();

    UE_LOGFMT(LogSystemMessageUI, Log, "시스템 메시지 종료");

    //@차단된 요청이 있다면 처리
    if (bIsBlocked)
    {
        bIsBlocked = false;
        UE_LOGFMT(LogSystemMessageUI, Log, "차단된 요청 해제 - 새 메시지 허용");
    }
}
#pragma endregion

//@Callbacks
#pragma region Callbacks
void UAPSystemMessage::OnRequestShowSystemMessage(const FString& SystemMessageText)
{
    DisplaySystemMesseage(SystemMessageText);
}
#pragma endregion

//@Utility(Setter, Getter,...etc)
#pragma region Utility
#pragma endregion