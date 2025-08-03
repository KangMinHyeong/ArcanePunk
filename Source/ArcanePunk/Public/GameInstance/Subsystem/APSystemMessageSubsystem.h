#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataAsset/APTextDataAsset.h"

#include "APSystemMessageSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSystemMessageSubsystem, Log, All)

//@전방 선언
#pragma region Forward Declaration
#pragma endregion

//@열거형
#pragma region Enums
#pragma endregion

//@구조체
#pragma region Structs
#pragma endregion

//@이벤트/델리게이트
#pragma region Delegates
DECLARE_DELEGATE_OneParam(FRequestShowSystemMessage, const FString&)
#pragma endregion

/**
 *  @UAPSystemMessageSubsystem
 *
 *  System Message 관리 수행하는 Subsystem
 *  이제 UAPTextDataAsset을 사용하여 더 현대적이고 안전한 데이터 관리를 수행합니다
 */
UCLASS()
class ARCANEPUNK_API UAPSystemMessageSubsystem : public UGameInstanceSubsystem
{
    //@친추 클래스
#pragma region Friend Class
#pragma endregion

    GENERATED_BODY()

    //@Defualt Setting
#pragma region Default Setting
public:
    // USubsystem interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
#pragma endregion

    //@Property/Info...etc
#pragma region Property or Subwidgets or Infos...etc
private:
    //@Data Asset 로딩 - DataTable에서 DataAsset으로 변경
    void LoadSystemMessageTextDataAsset();

    //@Text Data 캐싱 - DataAsset의 배열 기반 캐싱으로 변경
    void CacheSystemMessageTextDataAsset();

public:
    //@System Message 표시 시도
    UFUNCTION(BlueprintCallable, Category = "System Message")
    void TryShowSystemMessage(int64 MessageID);

protected:
    //@System Message Text Data Asset - DataTable에서 DataAsset으로 변경
    UPROPERTY()
    TObjectPtr<UAPTextDataAsset> SystemMessageTextDataAsset;

    //@System Message Text Data 캐시 - FTextData 타입 사용
    UPROPERTY()
    TMap<int64, FTextData> SystemMessageTextDataMap;

#pragma endregion

    //@Delegates
#pragma region Delegates
public:
    //@System Message 표시 요청 이벤트
    FRequestShowSystemMessage RequestShowSystemMessage;
#pragma endregion

    //@Callbacks
#pragma region Callbacks
public:
    UFUNCTION()
    void OnSomeGameplayStateEventOccurred(uint8 ID);
#pragma endregion

    //@Utility(Setter, Getter,...etc)
#pragma region Utility
#pragma endregion
};