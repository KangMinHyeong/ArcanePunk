#pragma once

#include "CoreMinimal.h"
#include "IPIEAuthorizer.h"
#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"

class FArcanePunkPIEAuthorizer : public IPIEAuthorizer
{
public:
	virtual bool RequestPIEPermission(bool bIsSimulateInEditor, FString& OutReason) const override;

private:
	mutable TMap<const UScriptStruct*, TSoftObjectPtr<UDataTable>> AllDataTablesByStruct;

	bool CollectAndValidateDataTables(FString& OutReason) const;
	bool CheckDuplicateStruct(const UScriptStruct* RowStruct, UDataTable* DataTable, FString& OutReason) const;
	static bool ValidateDataTableName(const UScriptStruct* RowStruct, UDataTable* DataTable, FString& OutReason);
	static bool ValidateEachRow(const UScriptStruct* RowStruct, UDataTable* DataTable, FString& OutReason);
};
