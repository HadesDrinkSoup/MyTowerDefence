#include "SpawnEnemy.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "001_Enemy/BaseEnemy.h"

// 构造函数：初始化默认值
ASpawnEnemy::ASpawnEnemy()
{
    // 启用每帧Tick函数
    PrimaryActorTick.bCanEverTick = true;

    // 初始化生成间隔为2秒
    SpawnInterval = 2.0f;

    // 初始化最大生成敌人数为10
    MaxEnemiesToSpawn = 10;

    // 默认自动开始生成
    bAutoStartSpawning = true;

    // 初始化已生成敌人数为0
    EnemiesSpawnedCount = 0;

    // 初始化生成状态为未开始
    bIsSpawning = false;
}
// 游戏开始时的初始化
void ASpawnEnemy::BeginPlay()
{
    Super::BeginPlay();
    GameModeRef = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
    // 如果设置了自动开始生成，则在游戏开始时启动生成器
    if (bAutoStartSpawning)
    {
        StartSpawning();
    }
}

// 游戏结束时的清理
// @param EndPlayReason - 游戏结束的原因
void ASpawnEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 停止生成敌人
    StopSpawning();

    // 清空委托绑定，防止内存泄漏
    OnEnemySpawned.Clear();

    // 调用父类结束游戏处理
    Super::EndPlay(EndPlayReason);
}

// 每帧更新函数
// @param DeltaTime - 距离上一帧的时间间隔
void ASpawnEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 开始生成敌人
// 设置计时器按间隔周期性地生成敌人
void ASpawnEnemy::StartSpawning()
{
    // 如果已经在生成中，则直接返回
    if (bIsSpawning) return;

    // 设置生成状态标志
    bIsSpawning = true;

    // 重置已生成敌人计数
    EnemiesSpawnedCount = 0;

    // 设置生成计时器：每隔SpawnInterval秒调用一次SpawnEnemy函数
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnEnemy::SpawnEnemy, SpawnInterval, true);
}

// 停止生成敌人
// 清除生成计时器并重置状态
void ASpawnEnemy::StopSpawning()
{
    // 如果不在生成状态，则直接返回
    if (!bIsSpawning) return;

    // 重置生成状态标志
    bIsSpawning = false;

    // 清除生成计时器
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

// 生成单个敌人实例
// 创建敌人Actor并初始化其路径，触发生成事件
void ASpawnEnemy::SpawnEnemy()
{
    // 前置条件检查：确保敌人类别、世界上下文和样条组件都存在
    if (!EnemyClass || !GetWorld() || !SplineActor)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] : 敌人类别、世界上下文或样条组件不存在"));
        return;
    }

    // 设置生成位置和旋转（使用生成器自身的位置和旋转）
    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();

    // 配置Actor生成参数
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;                    // 设置拥有者为当前生成器
    SpawnParams.Instigator = GetInstigator();    // 设置触发者

    // 在世界中生成敌人Actor
    ABaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

    // 检查敌人是否成功生成
    if (SpawnedEnemy)
    {
        // 第一步：初始化敌人的移动路径
        SpawnedEnemy->InitializeEnemyPath(SplineActor);

        // 第二步：广播敌人生成委托，通知其他系统有新敌人生成
        if (OnEnemySpawned.IsBound())
        {
            OnEnemySpawned.Broadcast(SpawnedEnemy);
        }

        // 第三步：管理生成的敌人
		GameModeRef->SpawnedEnemies.Add(SpawnedEnemy);  // 添加到游戏模式的已生成敌人数组
        SpawnedEnemy->OnEnemyDead.AddDynamic(GameModeRef, &ATowerDefenceGameMode::HandleEnemyDeath);// 绑定敌人死亡事件到游戏模式处理函数
        EnemiesSpawnedCount++;               // 增加生成计数

        // 检查是否达到最大生成数量限制
        if (MaxEnemiesToSpawn > 0 && EnemiesSpawnedCount >= MaxEnemiesToSpawn)
        {
            // 达到最大数量，停止生成
            UE_LOG(LogTemp, Warning, TEXT("[成功] 敌人：已达到最大生成数量 %d，停止生成。"), MaxEnemiesToSpawn);
            StopSpawning();
        }
    }
}