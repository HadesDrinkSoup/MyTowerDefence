// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "UObject/NoExportTypes.h"
#include "TurretDataTableManager.generated.h"

/**
 *
 */

class ABaseTurret;

USTRUCT(BlueprintType)
struct FTurretType : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Type")
	FName TypeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Type")
	TSoftObjectPtr<UDataTable> SubclassDataTable;
};

USTRUCT(BlueprintType)
struct FBaseTurretData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseTurret> TurretClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SellCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UNiagaraSystem*> GroundEffect;
};

UCLASS()
class MYTOWERDEFENCE_API UTurretDataTableManager : public UObject
{
	GENERATED_BODY()

public:
	const FTurretType* LoadedTurretTypeData;
	const FBaseTurretData* LoadedBaseTurretData;

public:
	UTurretDataTableManager();

	FTurretType* GetTurretTypeData(UDataTable* DataTable, FName RowName);
	FTurretType* GetTurretTypeDataByID(UDataTable* DataTable, FName TypeID);
	FBaseTurretData* GetTurretData(UDataTable* DataTable, FName RowName);

	TArray<FName> GetDataTableRowNames(UDataTable* DataTable);

	// 模板函数 - 实现放在头文件中
	template<typename T>
	bool ValidateDataTableIndex(int32 Index, const T* TurretData) const
	{
		if (!TurretData)
		{
			UE_LOG(LogTemp, Warning, TEXT("TurretData is null"));
			return false;
		}

		// 检查所有必需的数组是否都有足够的元素
		bool bValid = Index >= 0 &&
			Index < TurretData->UpgradeCost.Num() &&
			Index < TurretData->SellCost.Num() &&
			Index < TurretData->Damage.Num() &&
			Index < TurretData->AttackRange.Num() &&
			Index < TurretData->AttackSpeed.Num() &&
			Index < TurretData->TurretMesh.Num() &&
			Index < TurretData->GroundEffect.Num();

		return bValid;
	}
};