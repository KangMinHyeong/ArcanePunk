#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"

#include "APSystemMessage.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSystemMessageUI, Log, All)

//@전방 선언
#pragma region Forward Declaration
class UTextBlock;
class USizeBox;
class UWidgetAnimation;
#pragma endregion

//@열거형
#pragma region Enums
#pragma endregion

//@구조체
#pragma region Structs
#pragma endregion

//@이벤트/델리게이트
#pragma region Delegates
//@이벤트
DECLARE_MULTICAST_DELEGATE(FSystemMessageActivated);
DECLARE_MULTICAST_DELEGATE(FSystemMessageDeactivated);
#pragma endregion

/**
*   @USystemMessageConfig
*
*   System Message의 UI 표시 정보를 담은 데이터 에셋
*   유효성 검사 기능이 추가된 버전
*/
UCLASS(BlueprintType)
class ARCANEPUNK_API USystemMessageConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    USystemMessageConfig()
        : FontSize(24)
        , MaxRowSize(30)
        , MaxColumnSize(2)
        , LifeSpan(2)
        , FadeInTime(1)
        , FadeOutTime(1)
        , MinWidthScale(10.f)
        , MaxWidthScale(43.f)
        , MinHeightScale(2.5f)
        , MaxHeightScale(5.f)
    {
    }

protected:
    //@폰트 사이즈 (12~24)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 구성",
        meta = (ClampMin = "12", ClampMax = "24"))
    int32 FontSize;

    //@최대 행/열 길이
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 구성",
        meta = (ClampMin = "1"))
    int32 MaxRowSize;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 구성",
        meta = (ClampMin = "1"))
    int32 MaxColumnSize;

    //@지속 시간
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 구성",
        meta = (ClampMin = "1"))
    int32 LifeSpan;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 구성",
        meta = (ClampMin = "1"))
    int32 FadeInTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 구성",
        meta = (ClampMin = "1"))
    int32 FadeOutTime;

    //@최소/최대 크기 배율 (폰트 크기 기준)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 스케일",
        meta = (ClampMin = "0.1"))
    float MinWidthScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 스케일",
        meta = (ClampMin = "0.1"))
    float MaxWidthScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 스케일",
        meta = (ClampMin = "0.1"))
    float MinHeightScale;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | 스케일",
        meta = (ClampMin = "0.1"))
    float MaxHeightScale;

public:
    // Getter 함수들
    FORCEINLINE int32 GetFontSize() const { return FontSize; }
    FORCEINLINE int32 GetMaxRowSize() const { return MaxRowSize; }
    FORCEINLINE int32 GetMaxColumnSize() const { return MaxColumnSize; }
    FORCEINLINE int32 GetLifeSpan() const { return LifeSpan; }
    FORCEINLINE int32 GetFadeInTime() const { return FadeInTime; }
    FORCEINLINE int32 GetFadeOutTime() const { return FadeOutTime; }
    FORCEINLINE float GetTotalLifeTime() const { return static_cast<float>(FadeInTime + LifeSpan + FadeOutTime); }

    //@최소/최대 크기 계산 (폰트 기준)
    FORCEINLINE float GetMinWidth() const { return FontSize * MinWidthScale; }
    FORCEINLINE float GetMaxWidth() const { return FontSize * MaxWidthScale; }
    FORCEINLINE float GetMinHeight() const { return FontSize * MinHeightScale; }
    FORCEINLINE float GetMaxHeight() const { return FontSize * MaxHeightScale; }

    //@동적 크기 계산 (CharWidthMultiplier 제거)
    FORCEINLINE float CalculateWidth(int32 MaxLineLength) const
    {
        // 기본 문자 너비를 폰트 크기로 계산 (이전의 CharWidthMultiplier 제거)
        float DynamicWidth = MaxLineLength * FontSize;
        return FMath::Clamp(DynamicWidth, GetMinWidth(), GetMaxWidth());
    }

    FORCEINLINE float CalculateHeight(int32 LineCount) const
    {
        // 세로는 줄 수에 따라 최소~최대 사이 비례 배분
        float HeightRange = GetMaxHeight() - GetMinHeight();
        float HeightRatio = static_cast<float>(LineCount - 1) / static_cast<float>(MaxColumnSize - 1);
        return GetMinHeight() + (HeightRange * FMath::Clamp(HeightRatio, 0.0f, 1.0f));
    }

    // 유효성 검사 함수
    UFUNCTION(BlueprintCallable, Category = "시스템 메시지 | Validation")
    bool ValidateConfiguration(FString& OutErrorMessage) const;

#if WITH_EDITOR
protected:
    // 에디터에서 프로퍼티 변경 시 호출되는 함수
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    // 에디터 초기화 시 호출
    virtual void PostInitProperties() override;

private:
    // 내부 유효성 검사 함수들 - 기존 함수들
    bool ValidateMinMaxValues(FString& OutErrorMessage) const;
    bool ValidateNonZeroValues(FString& OutErrorMessage) const;

    // 새로운 검증 함수들 - 추가된 요구사항을 위한 함수들
    bool ValidateFadeTimesEquality(FString& OutErrorMessage) const;
    bool ValidateFontSizeRange(FString& OutErrorMessage) const;

    // 자동 수정 함수들 - 기존 함수들
    void FixMinMaxValues();
    void FixZeroValues();

    // 새로운 자동 수정 함수들 - 추가된 요구사항을 위한 함수들
    void FixFadeTimesEquality();
    void FixFontSizeRange();

    // 에디터 알림을 위한 헬퍼 함수 - 사용자에게 친화적인 피드백 제공
    void ShowEditorNotification(const FString& Message, bool bIsError = true) const;
#endif
};

/*
*   @UAPSystemMessage
* 
*   인/아웃 게임 상태를 알리는 System Mesage를 담는 UI
*/
UCLASS()
class ARCANEPUNK_API UAPSystemMessage : public UUserWidget
{

//@친추 클래스
#pragma region Friend Class
#pragma endregion

    GENERATED_BODY()

//@Defualt Setting
#pragma region Default Setting
public:
    virtual void NativeOnInitialized() override;
#pragma endregion

//@Property/Info...etc
#pragma region Property or Subwidgets or Infos...etc
private:
    void LoadSystemMessageConfig();

private:
    //@함수 추가 (private 섹션)
    void ApplyConfigSettings();

private:
    void UpdateFadeOpacity();

private:
    void StartFadeIn();
    void StartLifeSpan();
    void StartFadeOut();

public:
    void DisplaySystemMesseage(const FString& SystemMessageText);

private:
    FString ProcessTextForDisplay(const FString& InputText);
    void CompleteCurrentAnimation();

private:
void UpdateContentHighSize(const TArray<FString>& Lines);

private:
    void HideSystemMessage();

private:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* Text_Low;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* Text_High;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    USizeBox* Content_Low;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    USizeBox* Content_High;

    UPROPERTY(EditAnywhere)
    FSlateColor Notice_Color;
    UPROPERTY(EditAnywhere)
    FSlateColor Warning_Color;

private:
    //@타이머 핸들
    FTimerHandle LifeSpanTimerHandle;
    FTimerHandle FadeTimerHandle;

private:
    //@생명주기 상태
    bool bIsActive = false;
    bool bIsBlocked = false;

private:
    //@보간 관련
    float CurrentOpacity;
    float TargetOpacity;
    float FadeSpeed;

private:
    float AnimationStartTime;     // 애니메이션이 시작된 절대 시간 (게임 월드 기준)
    float AnimationDuration;      // 이번 애니메이션이 지속되어야 하는 총 시간 (초)
    float StartOpacity;           // 애니메이션 시작 시점의 투명도 값
    float EndOpacity;             // 애니메이션 완료 시점의 목표 투명도 값
    bool bIsCurrentlyAnimating;
#pragma endregion

//@Delegates
#pragma region Delegates
public:
    //@System Message 활성화 이벤트
    FSystemMessageActivated SystemMessageActivated;
    //@System Message 비활성화 이벤트
    FSystemMessageDeactivated SystemMessageDeactivated;
#pragma endregion

//@Callbacks
#pragma region Callbacks
protected:
    UFUNCTION()
        void OnRequestShowSystemMessage(const FString& SystemMessageText);
#pragma endregion

//@Utility(Setter, Getter,...etc)
#pragma region Utility
private:
    //@캐싱된 시스템 메시지 설정 데이터 에셋
    UPROPERTY()
        USystemMessageConfig* CachedMessageConfig;
#pragma endregion
};
