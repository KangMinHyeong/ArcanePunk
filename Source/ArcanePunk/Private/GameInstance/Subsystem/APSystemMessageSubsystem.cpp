#include "GameInstance/Subsystem/APSystemMessageSubsystem.h"
#include "Logging/StructuredLog.h"

#include "Engine/DataTable.h"

DEFINE_LOG_CATEGORY(LogSystemMessageSubsystem)

//@Defualt Setting
#pragma region Default Setting
void UAPSystemMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    LoadSystemMessageTextDataAsset();
}
#pragma endregion

//@Property/Info...etc
#pragma region Property or Subwidgets or Infos...etc
void UAPSystemMessageSubsystem::LoadSystemMessageTextDataAsset()
{
    static const TCHAR* TextDataAssetPath = TEXT("/Game/DataAsset/Text/DA_TextData_SystemMessage");

    SystemMessageTextDataAsset = LoadObject<UAPTextDataAsset>(nullptr, TextDataAssetPath);

    if (!SystemMessageTextDataAsset)
    {
        UE_LOGFMT(LogSystemMessageSubsystem, Error, "시스템 메시지 텍스트 데이터 에셋 로딩 실패: {0}", TextDataAssetPath);

        UE_LOGFMT(LogSystemMessageSubsystem, Warning, "기본 시스템 메시지 에셋이 생성되지 않았습니다. 에디터에서 해당 경로에 DataAsset을 생성해주세요.");
        return;
    }

    UE_LOGFMT(LogSystemMessageSubsystem, Log, "시스템 메시지 텍스트 데이터 에셋 로딩 성공: {0}", TextDataAssetPath);

    CacheSystemMessageTextDataAsset();
}

void UAPSystemMessageSubsystem::TryShowSystemMessage(int64 MessageID)
{
    const FTextData* MessageData = SystemMessageTextDataMap.Find(MessageID);

    if (!MessageData)
    {
        UE_LOGFMT(LogSystemMessageSubsystem, Warning, "ID {0}번 시스템 메시지를 찾을 수 없음", MessageID);

        return;
    }

    RequestShowSystemMessage.ExecuteIfBound(MessageData->GetText());
    UE_LOGFMT(LogSystemMessageSubsystem, Log, "시스템 메시지 표시 요청: {0}", MessageData->GetText());
}

void UAPSystemMessageSubsystem::OnSomeGameplayStateEventOccurred(uint8 ID)
{
    TryShowSystemMessage(static_cast<int64>(ID));
}
#pragma endregion

//@Callbacks
#pragma region Callbacks
#pragma endregion

//@Utility(Setter, Getter,...etc)
#pragma region Utility
void UAPSystemMessageSubsystem::CacheSystemMessageTextDataAsset()
{
    // DataAsset의 유효성을 먼저 확인합니다
    // 이는 "방어적 프로그래밍"의 핵심 원칙입니다
    if (!SystemMessageTextDataAsset)
    {
        UE_LOGFMT(LogSystemMessageSubsystem, Error, "시스템 메시지 텍스트 데이터 에셋이 null입니다");
        return;
    }

    SystemMessageTextDataMap.Empty();

    const TArray<FTextData>& TextDataList = SystemMessageTextDataAsset->TextDataList;

    SystemMessageTextDataMap.Reserve(TextDataList.Num());

    for (int32 Index = 0; Index < TextDataList.Num(); ++Index)
    {
        const FTextData& TextData = TextDataList[Index];

        if (TextData.GetID() > 0)
        {
            if (SystemMessageTextDataMap.Contains(TextData.GetID()))
            {
                UE_LOGFMT(LogSystemMessageSubsystem, Warning,
                    "중복된 ID {0}가 발견되었습니다. 인덱스 {1}의 데이터를 건너뜁니다.",
                    TextData.GetID(), Index);
                continue;
            }

            SystemMessageTextDataMap.Add(TextData.GetID(), TextData);

            UE_LOGFMT(LogSystemMessageSubsystem, Verbose,
                "ID {0} 캐싱 완료: {1}", TextData.GetID(), TextData.GetText());
        }
        else
        {
            UE_LOGFMT(LogSystemMessageSubsystem, Warning,
                "인덱스 {0}에 유효하지 않은 ID {1}가 있습니다. 캐시에서 제외됩니다.",
                Index, TextData.GetID());
        }
    }

    UE_LOGFMT(LogSystemMessageSubsystem, Log,
        "시스템 메시지 텍스트 데이터 캐싱 완료 - 원본: {0}개, 캐시됨: {1}개",
        TextDataList.Num(), SystemMessageTextDataMap.Num());

    if (TextDataList.Num() != SystemMessageTextDataMap.Num())
    {
        const int32 SkippedCount = TextDataList.Num() - SystemMessageTextDataMap.Num();
        UE_LOGFMT(LogSystemMessageSubsystem, Warning,
            "{0}개의 데이터가 유효하지 않아 캐시에서 제외되었습니다. DataAsset을 확인해주세요.",
            SkippedCount);
    }
}
#pragma endregion