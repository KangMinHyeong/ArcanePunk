#include "Tools/ArcanePunkPIEAuthorizer.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/MessageDialog.h"
#include "DataStructs/FValidatableTableRowBase.h"

bool FArcanePunkPIEAuthorizer::RequestPIEPermission(bool bIsSimulateInEditor, FString& OutReason) const
{
	return CollectAndValidateDataTables(OutReason);
}

bool FArcanePunkPIEAuthorizer::CollectAndValidateDataTables(FString& OutReason) const
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> DataTableAssets;
	FName DataTablePath(TEXT("/Game/DataTable"));
	AssetRegistryModule.Get().GetAssetsByPath(DataTablePath, DataTableAssets, true);
	FTopLevelAssetPath DataTableClassPath(TEXT("/Script/Engine"), TEXT("DataTable"));

	AllDataTablesByStruct.Empty();

	for (const FAssetData& Asset : DataTableAssets)
	{
		if (Asset.AssetClassPath == DataTableClassPath)
		{
			UDataTable* DataTable = Cast<UDataTable>(Asset.GetAsset());
			if (DataTable && DataTable->GetRowStruct())
			{
				const UScriptStruct* RowStruct = DataTable->GetRowStruct();
				if (!CheckDuplicateStruct(RowStruct, DataTable, OutReason)) return false;
				if (!ValidateDataTableName(RowStruct, DataTable, OutReason)) return false;
				if (!ValidateEachRow(RowStruct, DataTable, OutReason)) return false;
				AllDataTablesByStruct.Add(RowStruct, DataTable);
			}
		}
	}
	return true;
}

bool FArcanePunkPIEAuthorizer::CheckDuplicateStruct(const UScriptStruct* RowStruct, UDataTable* DataTable, FString& OutReason) const
{
	if (AllDataTablesByStruct.Contains(RowStruct))
	{
		UDataTable* ExistingTable = AllDataTablesByStruct[RowStruct].Get();
		FString StructName = RowStruct ? RowStruct->GetName() : TEXT("UnknownStruct");
		FString ExistingAssetName = ExistingTable ? ExistingTable->GetName() : TEXT("UnknownAsset");
		FString CurrentAssetName = DataTable->GetName();
		OutReason = FString::Printf(TEXT("[PIE Validation] 같은 RowStruct(%s)로 여러 DataTable이 등록되어 있습니다. 중복 애셋 이름: %s, %s"), *StructName, *ExistingAssetName, *CurrentAssetName);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(OutReason));
		return false;
	}
	return true;
}

bool FArcanePunkPIEAuthorizer::ValidateDataTableName(const UScriptStruct* RowStruct, UDataTable* DataTable, FString& OutReason)
{
	if (!RowStruct || !DataTable) return true;
	FString StructName = RowStruct->GetName();
	FString ExpectedTableName = StructName.StartsWith("F") ? StructName.RightChop(1) + TEXT("Table") : StructName + TEXT("Table");
	FString ActualTableName = DataTable->GetName();
	if (ActualTableName != ExpectedTableName)
	{
		OutReason = FString::Printf(TEXT("[PIE Validation] DataTable 이름 규칙 위반: 구조체(%s)에 대한 DataTable 이름은 %s 이어야 합니다. 현재 이름: %s"), *StructName, *ExpectedTableName, *ActualTableName);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(OutReason));
		return false;
	}
	return true;
}

bool FArcanePunkPIEAuthorizer::ValidateEachRow(const UScriptStruct* RowStruct, UDataTable* DataTable, FString& OutReason)
{
	const TMap<FName, uint8*>& RowMap = DataTable->GetRowMap();
	for (const TPair<FName, uint8*>& RowPair : RowMap)
	{
		const uint8* RawRow = RowPair.Value;
		const FTableRowBase* BaseRow = reinterpret_cast<const FTableRowBase*>(RawRow);

		if (RowStruct->IsChildOf(FValidatableTableRowBase::StaticStruct()))
		{
			const FValidatableTableRowBase* Validatable = reinterpret_cast<const FValidatableTableRowBase*>(BaseRow);
			if (!Validatable->ValidateAll(OutReason))
			{
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(OutReason));
				return false;
			}
		}
	}
	return true;
}
