#include "BaseTurret.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Engine/StaticMesh.h"

ABaseTurret::ABaseTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);

	GroundEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GroundEffect"));
	GroundEffectComponent->SetupAttachment(RootComponent);

	// 初始化默认值
	CurrentLevel = 0;
	UpgradeCost = 0;
	Damage = 0.0f;
	AttackRange = 0.0f;
	AttackSpeed = 0.0f;
	TurretDataTable = nullptr;
	LoadedTurretData = nullptr;
}

void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
	TurretsName = GetAllRowNames(TurretDataTable);
	// 如果有设置炮塔名称，则初始化
	if (TurretDataTable && !TurretName.IsNone())
	{
		InitializeTurretFromDataTable(TurretName);
	}
}

void ABaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseTurret::InitializeTurretFromDataTable(FName NewTurretName)
{
	if (!TurretDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔数据表为空"));
		return false;
	}

	if (NewTurretName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔行名称未设置"));
		return false;
	}

	// 更新炮塔名称
	TurretName = NewTurretName;

	LoadedTurretData = TurretDataTable->FindRow<FTurretData>(TurretName, TEXT("查找炮塔数据"));
	if (!LoadedTurretData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 未在数据表中找到炮塔数据: %s"), *TurretName.ToString());
		return false;
	}

	// 验证等级是否在有效范围内
	const int32 DataIndex = CurrentLevel;
	if (!ValidateDataTableIndex(DataIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 等级 %d 超出数据表范围"), CurrentLevel);
		return false;
	}

	// 应用数据表格中的属性
	UpgradeCost = LoadedTurretData->UpgradeCost[DataIndex];
	Damage = LoadedTurretData->Damage[DataIndex];
	AttackRange = LoadedTurretData->AttackRange[DataIndex];
	AttackSpeed = LoadedTurretData->AttackSpeed[DataIndex];

	// 设置炮塔网格体
	if (LoadedTurretData->TurretMesh.IsValidIndex(DataIndex) && LoadedTurretData->TurretMesh[DataIndex] && TurretMesh)
	{
		TurretMesh->SetStaticMesh(LoadedTurretData->TurretMesh[DataIndex]);
	}

	// 设置 Niagara 效果
	if (LoadedTurretData->GroundEffect.IsValidIndex(DataIndex) && LoadedTurretData->GroundEffect[DataIndex] && GroundEffectComponent)
	{
		GroundEffectComponent->SetAsset(LoadedTurretData->GroundEffect[DataIndex]);
		GroundEffectComponent->Activate();
	}

	UE_LOG(LogTemp, Log, TEXT("[成功] 炮塔已从表格数据加载: %s, 等级: %d"), *TurretName.ToString(), CurrentLevel);
	return true;
}

bool ABaseTurret::UpgradeTurret()
{
	if (!LoadedTurretData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 没有加载的炮塔数据"));
		return false;
	}

	const int32 NextLevel = CurrentLevel + 1;

	if (ValidateDataTableIndex(NextLevel))
	{
		if (GameModeRef->GetMoney() < LoadedTurretData->UpgradeCost[NextLevel])
		{
			UE_LOG(LogTemp, Warning, TEXT("[警告] 金币不足无法升级"));
			return false;
		}
		GameModeRef->SpendMoney(LoadedTurretData->UpgradeCost[NextLevel]);
		CurrentLevel = NextLevel;
		return InitializeTurretFromDataTable(TurretName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[警告] 已达到最大等级，无法升级"));
		return false;
	}
}

void ABaseTurret::SellOut()
{
	if (LoadedTurretData && LoadedTurretData->SellCost.IsValidIndex(CurrentLevel) && CurrentLevel > 0)
	{
		int32 Amount = LoadedTurretData->SellCost[CurrentLevel];
		GameModeRef->AddMoney(Amount);
		UE_LOG(LogTemp, Log, TEXT("出售成功获得金币 %d , 当前金币 %d "), Amount, GameModeRef->GetMoney());
		CurrentLevel = 0;
		TurretMesh->SetStaticMesh(nullptr);
		InitializeTurretFromDataTable(TurretName);
	}
}

TArray<FName> ABaseTurret::GetAllRowNames(UDataTable* Data) const
{
	TArray<FName> RowNames;
	if (!Data)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 数据表为空"));
	}
	const TMap<FName, uint8*>& RowMap = Data->GetRowMap();
	for(const auto Row : RowMap)
	{
		RowNames.Add(Row.Key);
	}
	return RowNames;
}

bool ABaseTurret::ValidateDataTableIndex(int32 Index) const
{
	if (!LoadedTurretData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 没有加载的炮塔数据"));
		return false;
	}
		
	return Index >= 0 &&
		Index < LoadedTurretData->UpgradeCost.Num() &&
		Index < LoadedTurretData->Damage.Num() &&
		Index < LoadedTurretData->AttackRange.Num() &&
		Index < LoadedTurretData->AttackSpeed.Num() &&
		Index < LoadedTurretData->TurretMesh.Num() &&
		Index < LoadedTurretData->GroundEffect.Num();
}