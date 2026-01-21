#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "APTextDataAsset.generated.h"

//@전방 선언
#pragma region Forward Declaration
#pragma endregion

//@열거형
#pragma region Enums
#pragma endregion

//@구조체
#pragma region Structs
/**
*   @FTextData
*
*   Text 정보를 담은 구조체
*/
USTRUCT(BlueprintType)
struct ARCANEPUNK_API FTextData
{
    GENERATED_BODY()

public:
    FTextData()
        : ID(0)
        , Text("")
    {
    }

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

/**
 * Text Data들을 관리하는 Primary Data Asset
 * 실시간 유효성 검사와 자동 오류 수정 기능을 포함
 *
 * 이 클래스는 게임에서 사용되는 모든 텍스트 데이터를 중앙 집중식으로 관리합니다.
 * 에디터에서 데이터를 수정할 때마다 자동으로 유효성을 검사하고,
 * 문제가 발견되면 즉시 수정하거나 개발자에게 알려줍니다.
 */
UCLASS(BlueprintType)
class ARCANEPUNK_API UAPTextDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UAPTextDataAsset();

    // 모든 텍스트 데이터를 담고 있는 배열
    // 이 배열이 변경될 때마다 자동으로 유효성 검사가 실행됩니다
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text Data",
        meta = (DisplayName = "텍스트 데이터 목록"))
    TArray<FTextData> TextDataList;

    // 빠른 검색을 위한 캐시 (에디터에서는 보이지 않음)
    // ID를 키로 사용하여 배열 인덱스를 빠르게 찾을 수 있습니다
    UPROPERTY(Transient)
    TMap<int64, int32> IDToIndexCache;

public:
    // ===============================
    // 블루프린트에서 사용할 수 있는 함수들
    // ===============================

    /**
     * 특정 ID의 텍스트 데이터를 찾아서 반환합니다
     * @param TextID 찾고자 하는 텍스트의 ID
     * @return 해당 ID의 텍스트 데이터 (찾지 못하면 기본값 반환)
     */
    UFUNCTION(BlueprintCallable, Category = "Text Data")
    FTextData GetTextDataByID(int64 TextID) const;

    /**
     * 특정 ID의 텍스트 데이터가 존재하는지 확인합니다
     * @param TextID 확인하고자 하는 텍스트 ID
     * @return 존재하면 true, 그렇지 않으면 false
     */
    UFUNCTION(BlueprintCallable, Category = "Text Data")
    bool HasTextData(int64 TextID) const;

    /**
     * 모든 텍스트 데이터의 ID 목록을 반환합니다
     * @return 모든 ID가 담긴 배열
     */
    UFUNCTION(BlueprintCallable, Category = "Text Data")
    TArray<int64> GetAllTextIDs() const;

    /**
     * 현재 저장된 텍스트 데이터의 총 개수를 반환합니다
     * @return 텍스트 데이터 개수
     */
    UFUNCTION(BlueprintCallable, Category = "Text Data")
    int32 GetTextDataCount() const { return TextDataList.Num(); }

    /**
     * 모든 텍스트 데이터의 유효성을 검사합니다
     * 이 함수는 블루프린트에서도 호출할 수 있어서 런타임 검증에도 사용할 수 있습니다
     * @param OutErrorMessage 오류가 있을 경우 상세한 오류 메시지
     * @return 모든 데이터가 유효하면 true, 문제가 있으면 false
     */
    UFUNCTION(BlueprintCallable, Category = "Text Data | Validation")
    bool ValidateAllTextData(FString& OutErrorMessage) const;

#if WITH_EDITOR
protected:
    // ===============================
    // 에디터 전용 함수들
    // ===============================

    /**
     * 에디터에서 프로퍼티가 변경될 때마다 호출되는 핵심 함수
     * 이 함수가 실시간 유효성 검사의 핵심입니다
     */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    /**
     * 에디터에서 에셋이 처음 로드될 때 호출되는 함수
     * 기존 데이터의 유효성을 확인하고 캐시를 구축합니다
     */
    virtual void PostInitProperties() override;

private:
    // ===============================
    // 내부 유틸리티 함수들
    // ===============================

    /**
     * ID 캐시를 업데이트합니다
     * 이 함수는 배열이 변경될 때마다 호출되어 빠른 검색을 가능하게 합니다
     */
    void UpdateIDCache();

    /**
     * 중복된 ID를 찾아서 자동으로 수정합니다
     * @return 중복이 발견되어 수정했으면 true, 그렇지 않으면 false
     */
    bool CheckAndFixDuplicateIDs();

    /**
     * 유효하지 않은 ID(0 또는 음수)를 찾아서 자동으로 수정합니다
     * @return 유효하지 않은 ID가 발견되어 수정했으면 true, 그렇지 않으면 false
     */
    bool CheckAndFixInvalidIDs();

    /**
     * 빈 텍스트나 너무 짧은 텍스트를 찾아서 경고합니다
     * @return 문제가 있는 텍스트가 발견되면 true, 그렇지 않으면 false
     */
    bool CheckEmptyOrShortTexts() const;

    /**
     * 사용되지 않은 고유한 ID를 생성합니다
     * @return 새로운 고유 ID
     */
    int64 GenerateUniqueID() const;

    /**
     * 에디터에 알림 메시지를 표시합니다
     * @param Message 표시할 메시지
     * @param bIsError true이면 오류 알림, false이면 정보 알림
     */
    void ShowEditorNotification(const FString& Message, bool bIsError = true) const;

    /**
     * 모든 텍스트 데이터를 초기 상태로 리셋합니다
     * 이 함수는 복구 불가능한 오류가 발생했을 때 안전장치 역할을 합니다
     */
    void ResetToSafeState();
#endif
};