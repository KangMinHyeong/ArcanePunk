
#include "UserInterface/Common/DisplayOnly/APSystemMessage.h"
#include "Logging/StructuredLog.h"

#include "GameInstance/APGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

#include "Kismet/GameplayStatics.h"
#include "GameInstance/Subsystem/APSystemMessageSubsystem.h"

//#include "Animation/WidgetAnimation.h"
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