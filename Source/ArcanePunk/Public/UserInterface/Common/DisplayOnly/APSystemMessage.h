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
#pragma endregion

/**
*   @USystemMessageConfig
*
*   System Message의 UI 표시 정보를 담은 데이터 에셋
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
        , LifeSpan(3)
        , FadeInTime(1)
        , FadeOutTime(1)
        , WidthScale(50.0f)
        , HeightScale(70.0f)
        , MinWidth(200.0f)
        , MaxWidth(1000.0f)
        , MinHeight(60.0f)
        , MaxHeight(100.0f)
    {
    }

protected:
    //@폰트 사이즈
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
        int32 FontSize;

    //@최대 행 길이
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
    int32 MaxRowSize;
    //@최대 열 길이
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
    int32 MaxColumnSize;
   
    //@최소/최대 크기
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Size")
    float MinWidth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Size")
    float MaxWidth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Size")
    float MinHeight;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Size")
    float MaxHeight;

    //@가로 배율 (폰트 크기 기준)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
    float WidthScale;

    //@세로 배율 (폰트 크기 기준)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
    float HeightScale;

    //@지속 시간 (초 단위)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
        int32 LifeSpan;
    
    //@페이드 인 소요 시간 (초 단위)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
        int32 FadeInTime;

    //@페이드 아웃 소요 시간 (초 단위)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "시스템 메시지 | Config")
        int32 FadeOutTime;

public:
    FORCEINLINE int32 GetFontSize() const { return FontSize; }
    FORCEINLINE int32 GetMaxRowSize() const { return MaxRowSize; }
    FORCEINLINE int32 GetMaxColumnSize() const { return MaxColumnSize; }
    FORCEINLINE float GetMinWidth() const { return MinWidth; }
    FORCEINLINE float GetMaxWidth() const { return MaxWidth; }
    FORCEINLINE float GetMinHeight() const { return MinHeight; }
    FORCEINLINE float GetMaxHeight() const { return MaxHeight; }
    FORCEINLINE float GetWidthScale() const { return WidthScale; }
    FORCEINLINE float GetHeightScale() const { return HeightScale; }
    FORCEINLINE int32 GetLifeSpan() const { return LifeSpan; }
    FORCEINLINE int32 GetFadeInTime() const { return FadeInTime; }
    FORCEINLINE int32 GetFadeOutTime() const { return FadeOutTime; }
    FORCEINLINE float GetTotalLifeTime() const { return static_cast<float>(FadeInTime + LifeSpan + FadeOutTime); }
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
    //@보간 관련
    float CurrentOpacity;
    float TargetOpacity;
    float FadeSpeed;


#pragma endregion

//@Delegates
#pragma region Delegates
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
