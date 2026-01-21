auto DataTable = DataTableGI->GetRowByStruct<FStringDataTable>(FName(*Name), Name);
auto DataTable = DataTableGI->GetRowByStruct<FStringDataTable>(RowName, RowName.ToString()); 