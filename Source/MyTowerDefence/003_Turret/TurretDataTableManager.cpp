#include "003_Turret/TurretDataTableManager.h"

UTurretDataTableManager::UTurretDataTableManager()
{
}

FTurretType* UTurretDataTableManager::GetTurretTypeData(UDataTable* DataTable, FName RowName)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 表格数据为空"));
        return nullptr;
    }
    FTurretType* RowData = DataTable->FindRow<FTurretType>(RowName, TEXT("GetTurretTypeData"));
    if (!RowData)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 未找到行名为 %s 的数据"), *RowName.ToString());
        return nullptr;
    }
    return RowData;
}

FTurretType* UTurretDataTableManager::GetTurretTypeDataByID(UDataTable* DataTable, FName TypeID)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 表格数据为空"));
        return nullptr;
    }
    const TMap<FName, uint8*>& RowMap = DataTable->GetRowMap();
    for (const auto& Entry : RowMap)
    {
        FTurretType* RowData = DataTable->FindRow<FTurretType>(Entry.Key, TEXT("GetTurretTypeDataByID"));
        if (RowData && RowData->TypeID == TypeID)
        {
            return RowData;
        }
    }
    return nullptr;
}

FBaseTurretData* UTurretDataTableManager::GetTurretData(UDataTable* DataTable, FName RowName)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 表格数据为空"));
        return nullptr;
    }
    FBaseTurretData* RowData = DataTable->FindRow<FBaseTurretData>(RowName, TEXT("GetTurretData"));
    if (!RowData)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 未找到行名为 %s 的数据"), *RowName.ToString());
        return nullptr;
    }
    return RowData;
}

TArray<FName> UTurretDataTableManager::GetDataTableRowNames(UDataTable* DataTable)
{
    TArray<FName> RowNames;

    if (!DataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 表格数据为空"));
        return RowNames;
    }

    const TMap<FName, uint8*>& RowMap = DataTable->GetRowMap();
    for (const auto& Entry : RowMap)
    {
        RowNames.Add(Entry.Key);
    }
    return RowNames;
}