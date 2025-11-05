#include "003_Turret/TurretSpawnerComponent.h"

UTurretSpawnerComponent::UTurretSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	TurretTypeDataTable = nullptr;
	GameModeRef = nullptr;
	DataTableManager = nullptr;
}

void UTurretSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTurretSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ATowerDefenceGameMode* UTurretSpawnerComponent::GetGameModeRef() const
{
	if (!GameModeRef)
	{
		UTurretSpawnerComponent* NonConstThis = const_cast<UTurretSpawnerComponent*>(this);
		NonConstThis->GameModeRef = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());

		if (GameModeRef)
		{
			UE_LOG(LogTemp, Log, TEXT("[信息] 懒加载获取游戏模式引用: %s"), *GameModeRef->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[错误] 懒加载无法获取游戏模式引用"));
		}
	}
	return GameModeRef;
}

UTurretDataTableManager* UTurretSpawnerComponent::GetDataTableManager() const
{
	if (!DataTableManager)
	{
		ATowerDefenceGameMode* Mode = GetGameModeRef();
		if (Mode)
		{
			UTurretSpawnerComponent* NonConstThis = const_cast<UTurretSpawnerComponent*>(this);
			NonConstThis->DataTableManager = Mode->TurretDataTableManager;

			if (DataTableManager)
			{
				UE_LOG(LogTemp, Log, TEXT("[信息] 懒加载获取数据表管理器"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[错误] 懒加载无法获取数据表管理器"));
			}
		}
	}
	return DataTableManager;
}

bool UTurretSpawnerComponent::EnsureInitialized() const
{
	return GetGameModeRef() && GetDataTableManager();
}

bool UTurretSpawnerComponent::SelectTurretType(FName TypeRowName)
{
	if (!TurretTypeDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法选择炮塔类型，组件未正确初始化"));
		return false;
	}

	// 获取类型数据
	FTurretType* TypeData = DataTableManager->GetTurretTypeData(TurretTypeDataTable, TypeRowName);
	if (!TypeData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法找到炮塔类型: %s"), *TypeRowName.ToString());
		return false;
	}

	SelectedType = *TypeData;
	UE_LOG(LogTemp, Log, TEXT("[信息] 选择炮塔类型: %s (ID: %s)"), *TypeRowName.ToString(), *SelectedType.TypeID.ToString());

	// 加载子类数据表
	UDataTable* SubclassDataTable = SelectedType.SubclassDataTable.LoadSynchronous();
	if (!SubclassDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法加载子类数据表"));
		return false;
	}

	// 获取可用的子类行
	AvailableSubclassRows = DataTableManager->GetDataTableRowNames(SubclassDataTable);

	// 默认选择第一个子类
	if (AvailableSubclassRows.Num() > 0)
	{
		return SelectTurretSubclass(AvailableSubclassRows[0]);
	}

	return true;
}

bool UTurretSpawnerComponent::SelectTurretSubclass(FName SubclassRowName)
{
	if (!EnsureInitialized() || !SelectedType.SubclassDataTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法选择炮塔子类，类型未选择或数据表无效"));
		return false;
	}

	UDataTable* SubclassDataTable = SelectedType.SubclassDataTable.LoadSynchronous();
	if (!SubclassDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法加载子类数据表"));
		return false;
	}

	// 获取子类数据
	FBaseTurretData* SubclassData = DataTableManager->GetTurretData(SubclassDataTable, SubclassRowName);
	if (!SubclassData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法找到炮塔子类: %s"), *SubclassRowName.ToString());
		return false;
	}

	SelectedSubclassData = *SubclassData;
	SelectedSubclassRowName = SubclassRowName;

	UE_LOG(LogTemp, Log, TEXT("[信息] 选择炮塔子类: %s"), *SubclassRowName.ToString());
	return true;
}

ABaseTurret* UTurretSpawnerComponent::SpawnTurret(FName TypeRowName, FName SubclassRowName)
{
	EnsureInitialized();
	// 选择类型
	if (!SelectTurretType(TypeRowName))
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法选择炮塔类型: %s"), *TypeRowName.ToString());
		return nullptr;
	}

	// 选择子类
	if (!SelectTurretSubclass(SubclassRowName))
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 无法选择炮塔子类: %s"), *SubclassRowName.ToString());
		return nullptr;
	}
	if (!CanSpawnTurret())
	{
		UE_LOG(LogTemp, Warning, TEXT("[警告] 无法生成炮塔，条件不满足"));
		return nullptr;
	}

	// 检查炮塔类是否有效
	if (!SelectedSubclassData.TurretClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔类无效"));
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 世界上下文无效"));
		return nullptr;
	}

	AActor* OwnerActor = GetOwner();
	FVector SpawnLocation = OwnerActor->GetActorLocation();
	FRotator SpawnRotation = OwnerActor->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;                    
	// 设置拥有者为当前生成器
	SpawnParams.Instigator = OwnerActor->GetInstigator();
	// 生成炮塔
	ABaseTurret* NewTurret = GetWorld()->SpawnActor<ABaseTurret>(SelectedSubclassData.TurretClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (NewTurret)
	{
		// 初始化炮塔数据
		if (NewTurret->InitializeTurretFromDataTable(SelectedSubclassRowName))
		{
			LastSpawnedTurret = NewTurret;
			UE_LOG(LogTemp, Log, TEXT("[信息] 成功生成炮塔: %s"),*SelectedSubclassRowName.ToString());

			return NewTurret;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔初始化失败"));
			NewTurret->Destroy();
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔生成失败"));
		return nullptr;
	}
}

bool UTurretSpawnerComponent::CanSpawnTurret() const
{
	if (!EnsureInitialized())
	{
		UE_LOG(LogTemp, Warning, TEXT("[警告] 组件未正确初始化"));
		return false;
	}

	if (!SelectedSubclassData.TurretClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[警告] 未选择有效的炮塔类"));
		return false;
	}

	// 检查是否有足够的金币（假设一级炮塔的生成成本是升级成本数组的第一个元素）
	if (SelectedSubclassData.UpgradeCost.Num() > 0)
	{
		int32 SpawnCost = SelectedSubclassData.UpgradeCost[0]; // 使用第一级的升级成本作为生成成本
		if (GameModeRef->GetMoney() < SpawnCost)
		{
			UE_LOG(LogTemp, Warning, TEXT("[警告] 金币不足，需要: %d, 当前: %d"), SpawnCost, GameModeRef->GetMoney());
			return false;
		}
	}

	return true;
}

int32 UTurretSpawnerComponent::GetSpawnCost() const
{
	if (SelectedSubclassData.UpgradeCost.Num() > 0)
	{
		return SelectedSubclassData.UpgradeCost[0]; // 使用第一级的升级成本作为生成成本
	}
	return 0;
}