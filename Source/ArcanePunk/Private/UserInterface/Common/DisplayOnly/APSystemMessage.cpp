
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

    //@폰트 크기 설정
    if (Text_High)
    {
        FSlateFontInfo FontInfo = Text_High->GetFont();
        FontInfo.Size = CachedMessageConfig->GetFontSize();
        Text_High->SetFont(FontInfo);
    }

    //@Content_High 크기 설정 (임의로 한 글자당 20픽셀, 한 줄당 40픽셀로 계산)
    if (Content_High)
    {
        const float Width = CachedMessageConfig->GetMaxRowSize() * 20.0f;
        const float Height = CachedMessageConfig->GetMaxColumnSize() * 40.0f;
        Content_High->SetWidthOverride(Width);
        Content_High->SetHeightOverride(Height);
    }

    UE_LOGFMT(LogSystemMessageUI, Log, "Config 설정 적용 완료 - 폰트크기: {0}, 최대크기: {1}x{2}",
        CachedMessageConfig->GetFontSize(),
        CachedMessageConfig->GetMaxRowSize(),
        CachedMessageConfig->GetMaxColumnSize());
}

void UAPSystemMessage::UpdateFadeOpacity()
{
    if (!Content_High) return;

    float DeltaTime = 0.016f;
    CurrentOpacity = FMath::FInterpTo(CurrentOpacity, TargetOpacity, DeltaTime, FadeSpeed);
    Content_High->SetRenderOpacity(CurrentOpacity);

    if (FMath::IsNearlyEqual(CurrentOpacity, TargetOpacity, 0.01f))
    {
        GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

        if (TargetOpacity > 0.5f) // Fade-In 완료
        {
            StartLifeSpan();
        }
        else // Fade-Out 완료
        {
            HideSystemMessage();
        }
    }
}

void UAPSystemMessage::StartFadeIn()
{
    if (!CachedMessageConfig || !Content_High) return;

    CurrentOpacity = 0.0f;
    TargetOpacity = 1.0f;
    FadeSpeed = 1.0f / CachedMessageConfig->GetFadeInTime();

    Content_High->SetRenderOpacity(CurrentOpacity);
    SetVisibility(ESlateVisibility::Visible);

    GetWorld()->GetTimerManager().SetTimer(
        FadeTimerHandle,
        this,
        &UAPSystemMessage::UpdateFadeOpacity,
        0.016f, // ~60fps
        true
    );
}

void UAPSystemMessage::StartLifeSpan()
{
    if (!CachedMessageConfig) return;

    CurrentOpacity = 1.0f;
    Content_High->SetRenderOpacity(1.0f);

    GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(
        LifeSpanTimerHandle,
        this,
        &UAPSystemMessage::StartFadeOut,
        CachedMessageConfig->GetLifeSpan(),
        false
    );
}

void UAPSystemMessage::StartFadeOut()
{
    if (!CachedMessageConfig || !Content_High) return;

    CurrentOpacity = 1.0f;
    TargetOpacity = 0.0f;
    FadeSpeed = 1.0f / CachedMessageConfig->GetFadeOutTime();

    GetWorld()->GetTimerManager().SetTimer(
        FadeTimerHandle,
        this,
        &UAPSystemMessage::UpdateFadeOpacity,
        0.016f,
        true
    );
}

//void UAPSystemMessage::DisplaySystemMesseage(EStringRowName StringRowName, bool bLowMessage, bool bWarning)
//{
//    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Low, StringRowName);
//    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_High, StringRowName);
//
//    if (bLowMessage)
//    {
//        Content_Low->SetVisibility(ESlateVisibility::Visible);
//        Content_High->SetVisibility(ESlateVisibility::Collapsed);
//    }
//    else
//    {
//        Content_Low->SetVisibility(ESlateVisibility::Collapsed);
//        Content_High->SetVisibility(ESlateVisibility::Visible);
//    }
//
//    if (bWarning)
//    {
//        Text_Low->SetColorAndOpacity(Warning_Color);
//        Text_High->SetColorAndOpacity(Warning_Color);
//        // red
//    }
//    else
//    {
//        Text_Low->SetColorAndOpacity(Notice_Color);
//        Text_High->SetColorAndOpacity(Notice_Color);
//        // green
//    }
//}
//
//void UAPSystemMessage::DisplaySystemMesseage(const FName& Message, bool bLowMessage, bool bWarning)
//{
//    Text_Low->SetText(FText::FromName(Message));
//    Text_Low->SetText(FText::FromName(Message));
//
//    if (bLowMessage)
//    {
//        Content_Low->SetVisibility(ESlateVisibility::Visible);
//        Content_High->SetVisibility(ESlateVisibility::Collapsed);
//    }
//    else
//    {
//        Content_Low->SetVisibility(ESlateVisibility::Collapsed);
//        Content_High->SetVisibility(ESlateVisibility::Visible);
//    }
//
//    if (bWarning)
//    {
//        Text_Low->SetColorAndOpacity(Warning_Color);
//        Text_High->SetColorAndOpacity(Warning_Color);
//        // red
//    }
//    else
//    {
//        Text_Low->SetColorAndOpacity(Notice_Color);
//        Text_High->SetColorAndOpacity(Notice_Color);
//        // green
//    }
//}

void UAPSystemMessage::DisplaySystemMesseage(const FString& SystemMessageText)
{
    if (!Text_High || !Content_High || !CachedMessageConfig)
    {
        UE_LOGFMT(LogSystemMessageUI, Error, "필수 컴포넌트가 초기화되지 않음");
        return;
    }

    //@텍스트 처리 및 개행
    FString ProcessedText = ProcessTextForDisplay(SystemMessageText);
    Text_High->SetText(FText::FromString(ProcessedText));

    //@Content_High 표시 및 페이드 시작
    Content_High->SetVisibility(ESlateVisibility::Visible);
    StartFadeIn();
}

FString UAPSystemMessage::ProcessTextForDisplay(const FString& InputText)
{
    if (!CachedMessageConfig) return InputText;

    const int32 MaxRowSize = CachedMessageConfig->GetMaxRowSize();
    const int32 MaxColumnSize = CachedMessageConfig->GetMaxColumnSize();

    TArray<FString> Lines;
    FString CurrentLine;

    //@문자별 처리
    for (int32 i = 0; i < InputText.Len(); ++i)
    {
        TCHAR CurrentChar = InputText[i];

        //@명시적 개행 처리
        if (CurrentChar == TEXT('\n'))
        {
            Lines.Add(CurrentLine);
            CurrentLine.Empty();
            continue;
        }

        //@현재 줄에 문자 추가
        CurrentLine += CurrentChar;

        //@자동 개행 조건 체크 (30자 또는 MaxRowSize 도달)
        if (CurrentLine.Len() >= 30 || CurrentLine.Len() >= MaxRowSize)
        {
            Lines.Add(CurrentLine);
            CurrentLine.Empty();
        }
    }

    //@마지막 줄 추가
    if (!CurrentLine.IsEmpty())
    {
        Lines.Add(CurrentLine);
    }

    //@최대 줄 수 제한
    if (Lines.Num() > MaxColumnSize)
    {
        Lines.SetNum(MaxColumnSize);
    }

    //@Content_High 크기 업데이트
    UpdateContentHighSize(Lines);

    //@최종 텍스트 생성
    return FString::Join(Lines, TEXT("\n"));
}

void UAPSystemMessage::UpdateContentHighSize(const TArray<FString>& Lines)
{
    if (!Content_High || !CachedMessageConfig) return;

    int32 MaxLineLength = 0;
    for (const FString& Line : Lines)
    {
        MaxLineLength = FMath::Max(MaxLineLength, Line.Len());
    }

    //@기본 크기 계산
    const float FontScale = CachedMessageConfig->GetFontSize() / 24.0f;
    float Width = MaxLineLength * CachedMessageConfig->GetWidthScale() * FontScale;
    float Height = Lines.Num() * CachedMessageConfig->GetHeightScale() * FontScale;

    //@최소/최대 크기 제한 적용
    Width = FMath::Clamp(Width, CachedMessageConfig->GetMinWidth(), CachedMessageConfig->GetMaxWidth());
    Height = FMath::Clamp(Height, CachedMessageConfig->GetMinHeight(), CachedMessageConfig->GetMaxHeight());

    // SizeBox의 부모 슬롯 크기 조정
    if (UPanelSlot* ParentSlot = Content_High->Slot)
    {
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ParentSlot))
        {
            CanvasSlot->SetSize(FVector2D(Width, Height));
        }
    }

    UE_LOGFMT(LogSystemMessageUI, Log, "SizeBox 부모 슬롯 크기: {0}x{1}", Width, Height);
}

void UAPSystemMessage::HideSystemMessage()
{
    SetVisibility(ESlateVisibility::Collapsed);
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
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