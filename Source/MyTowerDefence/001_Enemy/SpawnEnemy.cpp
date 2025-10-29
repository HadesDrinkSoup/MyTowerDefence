#include "SpawnEnemy.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ASpawnEnemy::ASpawnEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 设置默认生成参数
	SpawnInterval = 2.0f;
	MaxEnemiesToSpawn = 10;
	bAutoStartSpawning = true;
	EnemiesSpawnedCount = 0;
	bIsSpawning = false;
}

void ASpawnEnemy::StartSpawning()
{
	if (bIsSpawning) return;

	bIsSpawning = true;
	EnemiesSpawnedCount = 0;

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnEnemy::SpawnEnemy, SpawnInterval, true);

	UE_LOG(LogTemp, Warning, TEXT("Enemy spawning started with interval: %.2f seconds"), SpawnInterval);
}

void ASpawnEnemy::StopSpawning()
{
	if (!bIsSpawning) return;

	bIsSpawning = false;
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("Enemy spawning stopped. Total enemies spawned: %d"), EnemiesSpawnedCount);
}

void ASpawnEnemy::SpawnEnemy()
{
	if (EnemyClass && GetWorld())
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		ABaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedEnemy)
		{
			// 改进：确保委托在设置路径前广播，这样蓝图可以立即处理移动逻辑
			if (OnEnemySpawned.IsBound())
			{
				OnEnemySpawned.Broadcast(SpawnedEnemy);
			}

			// 设置敌人路径
			SpawnedEnemy->InitializeEnemyPath(SplineActor);

			// 添加到已生成敌人数组
			SpawnedEnemies.Add(SpawnedEnemy);
			EnemiesSpawnedCount++;

			// 如果达到最大生成数量，停止生成
			if (MaxEnemiesToSpawn > 0 && EnemiesSpawnedCount >= MaxEnemiesToSpawn)
			{
				StopSpawning();
			}
		}
	}
}

// Called when the game starts or when spawned
void ASpawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 如果设置为自动开始，则在游戏开始时启动生成
	if (bAutoStartSpawning)
	{
		StartSpawning();
	}
}

// 改进：添加委托清理
void ASpawnEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 停止生成计时器
	StopSpawning();

	// 清理委托绑定，防止悬空指针
	OnEnemySpawned.Clear();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASpawnEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}