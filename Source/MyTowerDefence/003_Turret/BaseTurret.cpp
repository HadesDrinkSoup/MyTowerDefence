#include "BaseTurret.h"

ABaseTurret::ABaseTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);

	GroundEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GroundEffect"));
	GroundEffectComponent->SetupAttachment(RootComponent);

	// 初始化默认值
	MaxLevel = 1;
	CurrentLevel = 1;
	UpgradeCost = 0;
	SellCost = 0;
	Damage = 0.0f;
	AttackRange = 0.0f;
	AttackSpeed = 0.0f;

	// 初始化指针
	GameModeRef = nullptr;
	TurretDataTableManager = nullptr;
}

void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ABaseTurret::BeginPlay - 开始初始化"));

	// 使用延迟初始化确保所有依赖项都已准备好
	GetWorld()->GetTimerManager().SetTimer(InitializeTimerHandle, this, &ABaseTurret::DelayedInitialize, 0.1f, false);
}

void ABaseTurret::DelayedInitialize()
{
	UE_LOG(LogTemp, Log, TEXT("ABaseTurret::DelayedInitialize - 执行延迟初始化"));

	// 获取游戏模式引用
	GameModeRef = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());

	if (GameModeRef)
	{
		UE_LOG(LogTemp, Log, TEXT("游戏模式引用获取成功: %s"), *GameModeRef->GetName());

		// 获取数据表管理器
		TurretDataTableManager = GameModeRef->TurretDataTableManager;

		if (TurretDataTableManager)
		{
			UE_LOG(LogTemp, Log, TEXT("数据表管理器获取成功"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[错误] 数据表管理器为空"));
		}

		// 验证关键引用
		if (ValidateCriticalRef())
		{
			if (TurretTypeDataTable)
			{
				UE_LOG(LogTemp, Log, TEXT("[信息] 炮塔类型数据表初始化成功"));;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[警告] 缺少炮塔类型名称或数据表"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 延迟初始化后游戏模式引用仍然为空"));
	}
}

void ABaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 懒加载获取游戏模式引用
ATowerDefenceGameMode* ABaseTurret::GetGameModeRef() const
{
	// 检查缓存
	if (!GameModeRef)
	{
		// 使用const_cast因为这是在const方法中
		ABaseTurret* ThisRef = const_cast<ABaseTurret*>(this);
		ThisRef->GameModeRef = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());

		if (GameModeRef)
		{
			UE_LOG(LogTemp, Log, TEXT("[信息] 懒加载获取游戏模式引用: %s"), *GameModeRef->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[错误] 懒加载无法获取游戏模式引用"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[信息] 游戏模式引用 %s 已设置"), *GameModeRef->GetName());
	}
	return GameModeRef;
}

// 懒加载获取数据表管理器
UTurretDataTableManager* ABaseTurret::GetTurretDataTableManager() const
{
	if (!TurretDataTableManager)
	{
		ATowerDefenceGameMode* Mode = GetGameModeRef();
		if (Mode)
		{
			ABaseTurret* NonConstThis = const_cast<ABaseTurret*>(this);
			NonConstThis->TurretDataTableManager = Mode->TurretDataTableManager;

			if (TurretDataTableManager)
			{
				UE_LOG(LogTemp, Log, TEXT("[信息] 懒加载已获取数据表管理器"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[错误] 懒加载无法获取数据表管理器"));
			}
		}
	}
	return TurretDataTableManager;
}

// 确保初始化完成
bool ABaseTurret::EnsureInitialized() const
{
	return GetGameModeRef() && GetTurretDataTableManager();
}

// 验证关键引用
bool ABaseTurret::ValidateCriticalRef() const
{
	if (!GetGameModeRef())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 游戏模式引用无效"));
		return false;
	}

	if (!GetTurretDataTableManager())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 数据表管理器无效"));
		return false;
	}

	return true;
}

// 必须提供 _Implementation 函数的默认实现
bool ABaseTurret::InitializeTurretFromDataTable_Implementation(FName NewTurretName)
{
	// 使用懒加载验证引用
	if (!EnsureInitialized())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 关键引用无效，无法初始化炮塔"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("ABaseTurret::InitializeTurretFromDataTable_Implementation - 基类默认实现，应该在子类中重写"));
	return false;
}

bool ABaseTurret::UpgradeTurret_Implementation()
{
	// 使用懒加载验证引用
	if (!EnsureInitialized())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 关键引用无效，无法升级炮塔"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("ABaseTurret::UpgradeTurret_Implementation - 基类默认实现，应该在子类中重写"));
	return false;
}

void ABaseTurret::SellOut_Implementation()
{
	// 使用懒加载验证引用
	if (!EnsureInitialized())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 关键引用无效，无法出售炮塔"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ABaseTurret::SellOut_Implementation - 基类默认实现，应该在子类中重写"));
}

// 调试函数
void ABaseTurret::DebugTurretSetup()
{
	UE_LOG(LogTemp, Log, TEXT("=== 炮塔调试信息 ==="));
	UE_LOG(LogTemp, Log, TEXT("游戏模式: %s"), GetGameModeRef() ? *GetGameModeRef()->GetName() : TEXT("空"));
	UE_LOG(LogTemp, Log, TEXT("数据表管理器: %s"), GetTurretDataTableManager() ? TEXT("有效") : TEXT("空"));
	UE_LOG(LogTemp, Log, TEXT("炮塔类型数据表: %s"), TurretTypeDataTable ? *TurretTypeDataTable->GetName() : TEXT("空"));
	UE_LOG(LogTemp, Log, TEXT("炮塔类型名称: %s"), *TurretTypeName.ToString());
	UE_LOG(LogTemp, Log, TEXT("当前等级: %d"), CurrentLevel);
	UE_LOG(LogTemp, Log, TEXT("===================="));
}