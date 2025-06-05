#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"

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
/**
*   @FTextData_SystemMessage
* 
*   System Message 정보를 담은 구조체
*/
USTRUCT(BlueprintType)
struct ARCANEPUNK_API FTextData_SystemMessage : public FTableRowBase
{
    GENERATED_BODY()

public:
    FTextData_SystemMessage()
        : ID(0)
        , Text(TEXT(""))
    {}

    //@System Mesage 고유의 ID
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "System Message")
        int64 ID;

    //@System Mesage 텍스트
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "System Message")
        FString Text;

public:
    FORCEINLINE int64 GetID() const { return ID; }
    FORCEINLINE const FString& GetText() const { return Text; }
};


#pragma endregion

//@이벤트/델리게이트
#pragma region Delegates
DECLARE_DELEGATE_OneParam(FRequestShowSystemMessage, const FString&)
#pragma endregion

/**
 *  @UAPSystemMessageSubsystem
 *
 *  System Message 관리 수행하는 Subsystem
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
    //@Data Table 로딩
    void LoadSystemMessageDataTable();

    //@Text Data 캐싱
    void CacheSystemMessageTextData();

protected:
    //@System Message 표시 시도
    UFUNCTION(BlueprintCallable, Category = "System Message")
    void TryShowSystemMessage(int64 MessageID);

protected:
    //@System Message Text Data Table
    UPROPERTY()
    TObjectPtr<UDataTable> SystemMessageTextDataTable;

    //@System Message Text Data 캐시
    UPROPERTY()
    TMap<int64, FTextData_SystemMessage> SystemMessageTextDataMap;
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
