#pragma once
#include "Engine/DataTable.h"
#include "Tools/APEditorErrorHelper.h"
#include "FValidatableTableRowBase.generated.h"

// 개별 검증 항목 구조체
USTRUCT()
struct FValidationEntry
{
    GENERATED_BODY();
    TFunction<bool()> Validator;
    FString ErrorMessage;
};

USTRUCT()
struct FValidatableTableRowBase : public FTableRowBase
{
    GENERATED_BODY()

protected:
    // 자식이 override하여 검증 함수와 메시지를 리스트에 추가
    virtual void AddValidationList(TArray<FValidationEntry>& OutValidationList) const PURE_VIRTUAL(FValidatableTableRowBase::GetValidationList, );

public:
    void ValidateAndCrashOnFailure() const
    {
        TArray<FValidationEntry> ValidationList;
        AddValidationList(ValidationList);

        for (const FValidationEntry& Entry : ValidationList)
        {
            if (!Entry.Validator())
            {
                APEditorErrorHelper::ReportErrorAndExitIfEditor(Entry.ErrorMessage);
            }
        }
    }
}; 