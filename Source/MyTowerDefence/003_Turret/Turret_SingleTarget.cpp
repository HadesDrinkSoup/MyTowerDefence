#include "003_Turret/Turret_SingleTarget.h"

bool ATurret_SingleTarget::InitializeTurretFromDataTable_Implementation(FName NewTurretName)
{
	// 使用基类的懒加载方法获取引用
	ATowerDefenceGameMode* Mode = GetGameModeRef();
	UTurretDataTableManager* Manager = GetTurretDataTableManager();

	if (!Mode || !Manager)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法获取必要的引用"));
		return false;
	}

	if (!SingleTargetTurretDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 单目标炮塔数据表为空"));
		return false;
	}

	if (NewTurretName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔名称为空"));
		return false;
	}

	// 更新炮塔名称
	TurretName = NewTurretName;
	// 获取数据
	SingleTargetTurretData = Manager->GetTurretData(SingleTargetTurretDataTable, NewTurretName);
	if (!SingleTargetTurretData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法从单目标炮塔数据表获取数据: %s"), *NewTurretName.ToString());
		return false;
	}

	const int32 DataIndex = CurrentLevel - 1;

	// 验证索引
	if (!Manager->ValidateDataTableIndex(DataIndex, SingleTargetTurretData))
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 等级 %d 超出单目标炮塔数据表范围"), CurrentLevel);
		return false;
	}

	// 应用单目标炮塔特有的数据
	MaxLevel = SingleTargetTurretData->MaxLevel;

	// 检查数组索引有效性
	if (SingleTargetTurretData->UpgradeCost.IsValidIndex(DataIndex))
		UpgradeCost = SingleTargetTurretData->UpgradeCost[DataIndex];

	if (SingleTargetTurretData->SellCost.IsValidIndex(DataIndex))
		SellCost = SingleTargetTurretData->SellCost[DataIndex];

	if (SingleTargetTurretData->Damage.IsValidIndex(DataIndex))
		Damage = SingleTargetTurretData->Damage[DataIndex];

	if (SingleTargetTurretData->AttackRange.IsValidIndex(DataIndex))
		AttackRange = SingleTargetTurretData->AttackRange[DataIndex];

	if (SingleTargetTurretData->AttackSpeed.IsValidIndex(DataIndex))
		AttackSpeed = SingleTargetTurretData->AttackSpeed[DataIndex];

	// 设置网格体
	if (SingleTargetTurretData->TurretMesh.IsValidIndex(DataIndex) && SingleTargetTurretData->TurretMesh[DataIndex] && TurretMesh)
	{
		TurretMesh->SetStaticMesh(SingleTargetTurretData->TurretMesh[DataIndex]);
	}

	// 设置特效
	if (SingleTargetTurretData->GroundEffect.IsValidIndex(DataIndex) && SingleTargetTurretData->GroundEffect[DataIndex] && GroundEffectComponent)
	{
		GroundEffectComponent->SetAsset(SingleTargetTurretData->GroundEffect[DataIndex]);
		GroundEffectComponent->Activate();
	}

	UE_LOG(LogTemp, Log, TEXT("[信息] 单目标炮塔已从表格数据加载: %s, 等级: %d"), *NewTurretName.ToString(), CurrentLevel + 1);
	return true;
}

bool ATurret_SingleTarget::UpgradeTurret_Implementation()
{
	// 使用基类的懒加载方法获取引用
	ATowerDefenceGameMode* Mode = GetGameModeRef();
	UTurretDataTableManager* Manager = GetTurretDataTableManager();

	if (!Mode || !Manager)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法获取必要的引用"));
		return false;
	}

	if (!SingleTargetTurretData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 没有加载的炮塔数据"));
		return false;
	}

	const int32 NextLevel = CurrentLevel + 1;
	const int32 DataIndex = NextLevel - 1;
	// 验证升级等级
	if (Manager->ValidateDataTableIndex(DataIndex, SingleTargetTurretData))
	{
		// 检查升级费用
		if (!SingleTargetTurretData->UpgradeCost.IsValidIndex(DataIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("[错误] 升级费用数据无效"));
			return false;
		}

		int32 RequiredMoney = SingleTargetTurretData->UpgradeCost[DataIndex];
		if (Mode->GetMoney() < RequiredMoney)
		{
			UE_LOG(LogTemp, Warning, TEXT("[警告] 金币不足无法升级，需要: %d, 当前: %d"), RequiredMoney, Mode->GetMoney());
			return false;
		}

		// 扣款并升级
		Mode->SpendMoney(RequiredMoney);
		CurrentLevel = NextLevel;

		UE_LOG(LogTemp, Log, TEXT("[信息] 炮塔升级到等级: %d"), CurrentLevel);
		return InitializeTurretFromDataTable(TurretName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[警告] 已达到最大等级 %d，无法升级"), SingleTargetTurretData->MaxLevel);
		return false;
	}
}

void ATurret_SingleTarget::SellOut_Implementation()
{
	// 使用基类的懒加载方法获取引用
	ATowerDefenceGameMode* Mode = GetGameModeRef();

	if (!Mode)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法获取游戏模式引用"));
		return;
	}

	const int32 DataIndex = CurrentLevel - 1;
	if (SingleTargetTurretData && SingleTargetTurretData->SellCost.IsValidIndex(DataIndex))
	{
		int32 SellValue = SingleTargetTurretData->SellCost[DataIndex];
		Mode->AddMoney(SellValue);
		UE_LOG(LogTemp, Log, TEXT("[信息] 炮塔已售出，获得金币: %d"), SellValue);
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法出售炮塔，数据无效"));
	}
}