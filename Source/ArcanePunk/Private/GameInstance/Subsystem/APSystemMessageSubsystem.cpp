#include "GameInstance/Subsystem/APSystemMessageSubsystem.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogSystemMessageSubsystem)

//@Defualt Setting
#pragma region Default Setting
void UAPSystemMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    LoadSystemMessageDataTable();
}
#pragma endregion

//@Property/Info...etc
#pragma region Property or Subwidgets or Infos...etc
void UAPSystemMessageSubsystem::LoadSystemMessageDataTable()
{
    // Text Data Table 로딩
    static const TCHAR* TextDataTablePath = TEXT("/Game/DataTable/Text/SystemMessageDataTable");
    SystemMessageTextDataTable = LoadObject<UDataTable>(nullptr, TextDataTablePath);

    if (!SystemMessageTextDataTable)
    {
        UE_LOGFMT(LogSystemMessageSubsystem, Error, "시스템 메시지 텍스트 데이터 테이블 로딩 실패: {0}", TextDataTablePath);
        return;
    }

    //@데이터 캐싱
    CacheSystemMessageTextData();
}

void UAPSystemMessageSubsystem::TryShowSystemMessage(int64 MessageID)
{
    auto* MessageData = SystemMessageTextDataMap.Find(MessageID);

    //@Text Data 찾기
    if (!MessageData)
    {
        UE_LOGFMT(LogSystemMessageSubsystem, Warning, "ID {0}번 시스템 메시지를 찾을 수 없음", MessageID);
    }

    //@System Message 그리기 요청 이벤트
    RequestShowSystemMessage.ExecuteIfBound(MessageData->GetText());
    UE_LOGFMT(LogSystemMessageSubsystem, Log, "시스템 메시지 표시 요청: {0}", MessageData->GetText());
}

void UAPSystemMessageSubsystem::OnSomeGameplayStateEventOccurred(uint8 ID)
{
    RequestShowSystemMessage.ExecuteIfBound(SystemMessageTextDataMap.Find(ID)->GetText());
}
#pragma endregion

//@Callbacks
#pragma region Callbacks
#pragma endregion

//@Utility(Setter, Getter,...etc)
#pragma region Utility
void UAPSystemMessageSubsystem::CacheSystemMessageTextData()
{
    if (!SystemMessageTextDataTable)
    {
        UE_LOGFMT(LogSystemMessageSubsystem, Error, "시스템 메시지 텍스트 데이터 테이블이 null입니다");
        return;
    }

    SystemMessageTextDataMap.Empty();

    // 모든 Row 데이터 캐싱
    TArray<FName> RowNames = SystemMessageTextDataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FSystemMessageData* RowData = SystemMessageTextDataTable->FindRow<FSystemMessageData>(RowName, "CacheSystemMessageTextData");
        if (RowData)
        {
            SystemMessageTextDataMap.Add(RowData->GetID(), *RowData);
        }
    }

    UE_LOGFMT(LogSystemMessageSubsystem, Log, "시스템 메시지 텍스트 데이터 {0}개 캐싱 완료", SystemMessageTextDataMap.Num());
}
#pragma endregion