#include "DataAsset/APTextDataAsset.h"

#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Misc/MessageDialog.h"
#include "Engine/Engine.h"
#endif

UAPTextDataAsset::UAPTextDataAsset()
{
    // 생성자에서는 기본 샘플 데이터를 추가합니다
    // 이는 개발자가 에셋을 처음 만들었을 때 "어떻게 사용해야 하는지" 알 수 있게 도와줍니다
    // 마치 새 문서를 열었을 때 템플릿이 제공되는 것과 같은 개념입니다

    if (TextDataList.Num() == 0)
    {
        // 첫 번째 샘플 데이터
        FTextData SampleData1;
        SampleData1.ID = 1001;
        SampleData1.Text = TEXT("환영합니다! 첫 번째 텍스트 데이터입니다.");
        TextDataList.Add(SampleData1);

        // 두 번째 샘플 데이터
        FTextData SampleData2;
        SampleData2.ID = 1002;
        SampleData2.Text = TEXT("이것은 두 번째 예시 텍스트입니다.");
        TextDataList.Add(SampleData2);
    }
}

// ===============================
// 공개 인터페이스 함수들
// 이 함수들은 블루프린트와 C++ 코드에서 모두 사용할 수 있습니다
// ===============================

FTextData UAPTextDataAsset::GetTextDataByID(int64 TextID) const
{
    // 캐시를 사용한 O(1) 검색 - 성능 최적화의 핵심입니다
    // 만약 1000개의 텍스트가 있어도 즉시 찾을 수 있습니다
    if (const int32* IndexPtr = IDToIndexCache.Find(TextID))
    {
        // 캐시에서 찾은 인덱스가 실제로 유효한지 안전성 검사
        if (TextDataList.IsValidIndex(*IndexPtr))
        {
            return TextDataList[*IndexPtr];
        }
    }

    // 캐시에 없거나 인덱스가 유효하지 않은 경우 직접 검색
    // 이는 안전장치 역할을 합니다 - 캐시가 손상되더라도 데이터를 찾을 수 있습니다
    for (const FTextData& Data : TextDataList)
    {
        if (Data.ID == TextID)
        {
            return Data;
        }
    }

    // 찾지 못한 경우 빈 데이터 반환
    // 이는 "null 참조 오류"를 방지하는 안전한 패턴입니다
    UE_LOG(LogTemp, Warning, TEXT("ID %lld에 해당하는 텍스트 데이터를 찾을 수 없습니다."), TextID);
    return FTextData();
}

bool UAPTextDataAsset::HasTextData(int64 TextID) const
{
    // 단순히 캐시에 해당 ID가 있는지 확인
    // 이는 매우 빠른 존재 여부 확인 방법입니다
    return IDToIndexCache.Contains(TextID);
}

TArray<int64> UAPTextDataAsset::GetAllTextIDs() const
{
    // 모든 ID를 수집해서 반환
    // 이 함수는 UI에서 드롭다운 목록을 만들 때 유용합니다
    TArray<int64> IDs;
    IDs.Reserve(TextDataList.Num()); // 메모리 재할당을 방지하는 최적화

    for (const FTextData& Data : TextDataList)
    {
        if (Data.ID > 0) // 유효한 ID만 포함
        {
            IDs.Add(Data.ID);
        }
    }

    return IDs;
}

bool UAPTextDataAsset::ValidateAllTextData(FString& OutErrorMessage) const
{
    OutErrorMessage.Empty();

    // 1단계: 배열이 완전히 비어있는지 확인
    if (TextDataList.Num() == 0)
    {
        OutErrorMessage = TEXT("텍스트 데이터가 하나도 없습니다.");
        return false;
    }

    // 2단계: 각 데이터의 기본 유효성 검사
    for (int32 i = 0; i < TextDataList.Num(); ++i)
    {
        const FTextData& Data = TextDataList[i];

        // ID 유효성 검사
        if (Data.ID <= 0)
        {
            OutErrorMessage = FString::Printf(
                TEXT("인덱스 %d의 데이터에 유효하지 않은 ID가 있습니다. (ID: %lld)"),
                i, Data.ID
            );
            return false;
        }

        // 텍스트 유효성 검사
        if (Data.Text.IsEmpty())
        {
            OutErrorMessage = FString::Printf(
                TEXT("인덱스 %d의 데이터에 빈 텍스트가 있습니다. (ID: %lld)"),
                i, Data.ID
            );
            return false;
        }

        // 텍스트 길이 검사 (너무 짧은 텍스트 방지)
        if (Data.Text.Len() < 3)
        {
            OutErrorMessage = FString::Printf(
                TEXT("인덱스 %d의 데이터 텍스트가 너무 짧습니다. (ID: %lld, Text: '%s')"),
                i, Data.ID, *Data.Text
            );
            return false;
        }
    }

    // 3단계: 중복 ID 검사 - 이는 매우 중요한 무결성 검사입니다
    TSet<int64> UniqueIDs;
    for (int32 i = 0; i < TextDataList.Num(); ++i)
    {
        const int64 CurrentID = TextDataList[i].ID;
        if (UniqueIDs.Contains(CurrentID))
        {
            OutErrorMessage = FString::Printf(
                TEXT("ID %lld가 중복됩니다. (인덱스 %d)"), CurrentID, i
            );
            return false;
        }
        UniqueIDs.Add(CurrentID);
    }

    // 모든 검사를 통과한 경우
    OutErrorMessage = FString::Printf(
        TEXT("모든 텍스트 데이터가 유효합니다. (총 %d개)"), TextDataList.Num()
    );
    return true;
}

#if WITH_EDITOR

// ===============================
// 에디터 전용 함수들
// 이 함수들은 개발 중에만 실행되고, 최종 게임 빌드에는 포함되지 않습니다
// ===============================

void UAPTextDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    // 부모 클래스의 처리를 먼저 수행 - 이는 Unreal Engine의 표준 패턴입니다
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // 변경된 프로퍼티가 있는지 확인
    if (PropertyChangedEvent.Property != nullptr)
    {
        const FName PropertyName = PropertyChangedEvent.Property->GetFName();

        // TextDataList 배열이나 그 내부 요소가 변경되었는지 확인
        // 이것이 바로 "실시간 검증"의 핵심 조건입니다
        if (PropertyName == GET_MEMBER_NAME_CHECKED(UAPTextDataAsset, TextDataList) ||
            PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAPTextDataAsset, TextDataList))
        {
            UE_LOG(LogTemp, Log, TEXT("APTextDataAsset: 텍스트 데이터가 변경되었습니다. 유효성 검사를 시작합니다..."));

            // 단계별 검증과 자동 수정을 수행합니다
            // 각 단계는 독립적이지만 순서가 중요합니다

            // 1단계: ID 캐시 업데이트 (다른 검사들의 기반이 됩니다)
            UpdateIDCache();

            // 2단계: 유효하지 않은 ID 수정 (0이나 음수를 양수로 변경)
            bool bFixedInvalidIDs = CheckAndFixInvalidIDs();

            // 3단계: 중복 ID 검사 및 수정 (데이터 무결성의 핵심)
            bool bFixedDuplicates = CheckAndFixDuplicateIDs();

            // 4단계: 텍스트 품질 검사 (빈 텍스트나 너무 짧은 텍스트)
            bool bHasTextIssues = CheckEmptyOrShortTexts();

            // 수정이 이루어진 경우 캐시를 다시 업데이트
            if (bFixedInvalidIDs || bFixedDuplicates)
            {
                UpdateIDCache();

                // 변경사항을 에디터에 알려서 UI가 업데이트되도록 합니다
                MarkPackageDirty();
            }

            // 사용자에게 수행된 작업에 대한 피드백 제공
            if (bFixedInvalidIDs)
            {
                ShowEditorNotification(TEXT("유효하지 않은 ID가 발견되어 자동으로 수정했습니다."), false);
            }

            if (bFixedDuplicates)
            {
                ShowEditorNotification(TEXT("중복된 ID가 발견되어 자동으로 수정했습니다."), false);
            }

            if (bHasTextIssues)
            {
                ShowEditorNotification(TEXT("비어있거나 너무 짧은 텍스트가 있습니다. 확인해주세요."), true);
            }

            // 최종 전체 유효성 검사
            FString ValidationMessage;
            bool bIsValid = ValidateAllTextData(ValidationMessage);

            if (!bIsValid && !bFixedInvalidIDs && !bFixedDuplicates)
            {
                // 자동 수정되지 않은 문제가 있는 경우에만 오류 표시
                ShowEditorNotification(ValidationMessage, true);
            }
            else if (bIsValid && !bFixedInvalidIDs && !bFixedDuplicates && !bHasTextIssues)
            {
                // 모든 것이 완벽한 경우 간단한 성공 메시지
                UE_LOG(LogTemp, Verbose, TEXT("APTextDataAsset: %s"), *ValidationMessage);
            }
        }
    }
}

void UAPTextDataAsset::PostInitProperties()
{
    Super::PostInitProperties();

    // 에디터에서 에셋을 처음 로드할 때도 검증 수행
    // 이는 다른 개발자가 만든 에셋을 열었을 때도 안전성을 보장합니다
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        UpdateIDCache();

        FString ValidationMessage;
        if (!ValidateAllTextData(ValidationMessage))
        {
            UE_LOG(LogTemp, Warning, TEXT("APTextDataAsset 로드 시 유효성 검사 실패: %s"), *ValidationMessage);

            // 심각한 문제가 있는 경우 안전 상태로 리셋
            ResetToSafeState();
        }
    }
}

void UAPTextDataAsset::UpdateIDCache()
{
    // 기존 캐시를 완전히 초기화
    IDToIndexCache.Empty();

    // 메모리 효율성을 위한 사전 할당
    IDToIndexCache.Reserve(TextDataList.Num());

    // 새로운 캐시 구축
    for (int32 i = 0; i < TextDataList.Num(); ++i)
    {
        const FTextData& Data = TextDataList[i];
        if (Data.ID > 0)  // 유효한 ID만 캐시에 추가
        {
            IDToIndexCache.Add(Data.ID, i);
        }
    }

    UE_LOG(LogTemp, Verbose, TEXT("APTextDataAsset: ID 캐시 업데이트 완료. 총 %d개의 항목이 캐시되었습니다."), IDToIndexCache.Num());
}

bool UAPTextDataAsset::CheckAndFixInvalidIDs()
{
    bool bFixedAny = false;

    // 배열을 순회하면서 유효하지 않은 ID를 찾습니다
    for (int32 i = 0; i < TextDataList.Num(); ++i)
    {
        FTextData& Data = TextDataList[i];

        // ID가 0이거나 음수인 경우 수정이 필요합니다
        if (Data.ID <= 0)
        {
            int64 OriginalID = Data.ID;
            Data.ID = GenerateUniqueID();

            UE_LOG(LogTemp, Warning,
                TEXT("APTextDataAsset: 인덱스 %d의 유효하지 않은 ID를 수정했습니다. %lld -> %lld"),
                i, OriginalID, Data.ID
            );

            bFixedAny = true;
        }
    }

    return bFixedAny;
}

bool UAPTextDataAsset::CheckAndFixDuplicateIDs()
{
    bool bFixedAny = false;
    TSet<int64> UsedIDs;

    // 각 항목을 순회하면서 중복 검사를 수행합니다
    for (int32 i = 0; i < TextDataList.Num(); ++i)
    {
        FTextData& Data = TextDataList[i];

        // 이미 사용된 ID인지 확인
        if (UsedIDs.Contains(Data.ID))
        {
            // 중복이 발견된 경우 새로운 고유 ID 생성
            int64 OriginalID = Data.ID;
            Data.ID = GenerateUniqueID();

            // 새로 생성된 ID도 UsedIDs에 추가해야 나중에 중복되지 않습니다
            UsedIDs.Add(Data.ID);

            UE_LOG(LogTemp, Warning,
                TEXT("APTextDataAsset: 인덱스 %d의 중복 ID를 수정했습니다. %lld -> %lld"),
                i, OriginalID, Data.ID
            );

            bFixedAny = true;
        }
        else
        {
            // 중복이 아닌 경우 사용된 ID 목록에 추가
            UsedIDs.Add(Data.ID);
        }
    }

    return bFixedAny;
}

bool UAPTextDataAsset::CheckEmptyOrShortTexts() const
{
    bool bFoundIssues = false;

    for (int32 i = 0; i < TextDataList.Num(); ++i)
    {
        const FTextData& Data = TextDataList[i];

        if (Data.Text.IsEmpty())
        {
            UE_LOG(LogTemp, Warning,
                TEXT("APTextDataAsset: 인덱스 %d에 빈 텍스트가 있습니다. (ID: %lld)"),
                i, Data.ID
            );
            bFoundIssues = true;
        }
        else if (Data.Text.Len() < 3)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("APTextDataAsset: 인덱스 %d의 텍스트가 너무 짧습니다. (ID: %lld, Text: '%s')"),
                i, Data.ID, *Data.Text
            );
            bFoundIssues = true;
        }
    }

    return bFoundIssues;
}

int64 UAPTextDataAsset::GenerateUniqueID() const
{
    // 1001부터 시작해서 사용되지 않은 ID를 찾습니다
    // 왜 1001부터 시작하는가? 1000 이하는 시스템 예약용으로 남겨두기 위함입니다
    int64 NewID = 1001;

    // 현재 사용 중인 모든 ID를 수집합니다
    TSet<int64> ExistingIDs;
    for (const FTextData& Data : TextDataList)
    {
        if (Data.ID > 0)
        {
            ExistingIDs.Add(Data.ID);
        }
    }

    // 사용되지 않은 ID를 찾을 때까지 증가시킵니다
    while (ExistingIDs.Contains(NewID))
    {
        NewID++;

        // 무한루프 방지 안전장치 (이론적으로는 필요 없지만 안전을 위해)
        if (NewID > 999999)
        {
            UE_LOG(LogTemp, Error, TEXT("APTextDataAsset: ID 생성 중 예상치 못한 오류가 발생했습니다."));
            break;
        }
    }

    return NewID;
}

void UAPTextDataAsset::ShowEditorNotification(const FString& Message, bool bIsError) const
{
    // Unreal Engine의 에디터 알림 시스템을 사용합니다
    // 이는 사용자에게 비침습적인 방식으로 정보를 전달하는 현대적인 UI 패턴입니다

    FNotificationInfo Info(FText::FromString(Message));
    Info.bFireAndForget = true;          // 알림이 자동으로 사라지도록 설정
    Info.FadeOutDuration = 2.0f;         // 페이드 아웃 시간
    Info.ExpireDuration = 4.0f;          // 총 표시 시간

    // 메시지 유형에 따라 다른 아이콘 사용
    if (bIsError)
    {
        Info.Image = FAppStyle::GetBrush(TEXT("MessageLog.Error"));
    }
    else
    {
        Info.Image = FAppStyle::GetBrush(TEXT("MessageLog.Info"));
    }

    // 알림을 화면에 표시합니다
    FSlateNotificationManager::Get().AddNotification(Info);

    // 로그에도 기록해서 나중에 추적할 수 있게 합니다
    if (bIsError)
    {
        UE_LOG(LogTemp, Warning, TEXT("APTextDataAsset 알림: %s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("APTextDataAsset 정보: %s"), *Message);
    }
}

void UAPTextDataAsset::ResetToSafeState()
{
    // 이 함수는 "비상 탈출구" 역할을 합니다
    // 데이터가 너무 심하게 손상되어 복구가 불가능한 경우에만 호출됩니다

    UE_LOG(LogTemp, Error, TEXT("APTextDataAsset: 심각한 데이터 오류로 인해 안전 상태로 리셋합니다."));

    // 기존 데이터를 모두 제거하고 기본 샘플 데이터로 초기화
    TextDataList.Empty();
    IDToIndexCache.Empty();

    // 안전한 기본 데이터 추가
    FTextData SafeData;
    SafeData.ID = 1001;
    SafeData.Text = TEXT("기본 텍스트 데이터 (안전 모드)");
    TextDataList.Add(SafeData);

    // 캐시 재구축
    UpdateIDCache();

    // 변경사항을 저장하도록 표시
    MarkPackageDirty();

    // 사용자에게 알림
    ShowEditorNotification(TEXT("심각한 오류로 인해 데이터가 안전 상태로 리셋되었습니다. 백업에서 복구하는 것을 권장합니다."), true);
}

#endif // WITH_EDITOR