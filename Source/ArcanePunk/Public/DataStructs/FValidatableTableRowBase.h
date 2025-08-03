#pragma once
#include "Engine/DataTable.h"
#include "FValidatableTableRowBase.generated.h"

USTRUCT()
struct FValidationEntry
{
    GENERATED_BODY()

    TFunction<bool()> Validator;
    FString ErrorMessage;
};

USTRUCT()
struct FValidatableTableRowBase : public FTableRowBase
{
    GENERATED_BODY()

protected:
    virtual void AddValidationList(TArray<FValidationEntry>& OutValidationList) const {};

public:
    bool ValidateAll(FString& OutErrorMessage) const
    {
        TArray<FValidationEntry> ValidationList;
        AddValidationList(ValidationList);

        for (const FValidationEntry& Entry : ValidationList)
        {
            if (!Entry.Validator())
            {
                OutErrorMessage = Entry.ErrorMessage;
                return false;
            }
        }
        return true;
    }
};
