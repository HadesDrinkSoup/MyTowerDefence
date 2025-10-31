#include "003_Turret/BaseTurret.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
ABaseTurret::ABaseTurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建根组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 创建并设置炮塔网格体组件
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);

	// 创建 Niagara 效果组件
	GroundEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GroundEffect"));
	GroundEffectComponent->SetupAttachment(RootComponent);

	// 初始化默认值
	Level = 1;
	UpgradeCost = 30;
	Damage = 10.0f;
	AttackRange = 500.0f;
	AttackSpeed = 1.0f;
	TurretDataTable = nullptr;
}

// Called when the game starts or when spawned
void ABaseTurret::BeginPlay()
{
	Super::BeginPlay();

	// 自动从数据表格初始化
	if (TurretDataTable && !TurretRowName.IsNone())
	{
		InitializeTurretFromDataTable();
	}
}

// Called every frame
void ABaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseTurret::InitializeTurretFromDataTable()
{
	if (!TurretDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔数据表为空"));
		return false;
	}

	if (TurretRowName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 炮塔行名称未设置"));
		return false;
	}

	LoadedTurretData = TurretDataTable->FindRow<FTurretData>(TurretRowName, TEXT("查找炮塔数据"));
	if (!LoadedTurretData)
	{
		UE_LOG(LogTemp, Error, TEXT("[错误] 未在数据表中找到炮塔数据: %s"), *TurretRowName.ToString());
		return false;
	}

	// 应用数据表格中的属性
	Level = LoadedTurretData->Level;
	UpgradeCost = LoadedTurretData->UpgradeCost;
	Damage = LoadedTurretData->Damage;
	AttackRange = LoadedTurretData->AttackRange;
	AttackSpeed = LoadedTurretData->AttackSpeed;

	// 设置炮塔网格体
	if (LoadedTurretData->TurretMesh && TurretMesh)
	{
		TurretMesh->SetStaticMesh(LoadedTurretData->TurretMesh);
	}

	// 设置 Niagara 效果
	if (LoadedTurretData->GroundEffect && GroundEffectComponent)
	{
		GroundEffectComponent->SetAsset(LoadedTurretData->GroundEffect);
		GroundEffectComponent->Activate(); // 激活效果
	}

	UE_LOG(LogTemp, Log, TEXT("[成功] 炮塔已从表格数据初始化: %s"), *TurretRowName.ToString());
	return true;
}