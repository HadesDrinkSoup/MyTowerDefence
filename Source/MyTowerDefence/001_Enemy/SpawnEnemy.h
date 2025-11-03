// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "001_Enemy/BaseEnemy.h"
#include "002_GameMode/TowerDefenceGameMode.h"
#include "SpawnEnemy.generated.h"

// 敌人生成委托声明
// 当敌人被成功生成时广播，传递生成的敌人实例
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawned, ABaseEnemy*, Enemy);

UCLASS()
class MYTOWERDEFENCE_API ASpawnEnemy : public AActor
{
    GENERATED_BODY()

public:
    // 构造函数
    ASpawnEnemy();

public:
    // 要生成的敌人类别，可在编辑器和蓝图中设置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEnemy")
    TSubclassOf<ABaseEnemy> EnemyClass;

    // 生成敌人的时间间隔（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnInterval;

    // 最大生成敌人数量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    int32 MaxEnemiesToSpawn;

    // 是否在游戏开始时自动开始生成敌人
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    bool bAutoStartSpawning;

    // 敌人生成委托
    // 当每个敌人被生成时广播此委托
    UPROPERTY(BlueprintAssignable, Category = "Spawning")
    FOnEnemySpawned OnEnemySpawned;

protected:
    ATowerDefenceGameMode* GameModeRef;

    // 样条组件引用，用于为生成的敌人设置移动路径
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Path")
    AActor* SplineActor;

    // 已生成敌人的计数
    UPROPERTY(BlueprintReadOnly, Category = "Spawning")
    int32 EnemiesSpawnedCount;

    // 生成状态标志，表示是否正在生成敌人
    UPROPERTY(BlueprintReadOnly, Category = "Spawning")
    bool bIsSpawning;

    // 生成计时器句柄，用于控制间隔生成
    FTimerHandle SpawnTimerHandle;

protected:
    // 游戏开始时调用
    virtual void BeginPlay() override;

    // 游戏结束时调用，清理资源
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    // 每帧更新函数
    virtual void Tick(float DeltaTime) override;

public:
    // 开始生成敌人
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void StartSpawning();

    // 停止生成敌人
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void StopSpawning();

    // 生成单个敌人实例
    UFUNCTION(BlueprintCallable, Category = "SpawnEnemy")
    void SpawnEnemy();
};