// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "001_Enemy/BaseEnemy.h"
#include "SpawnEnemy.generated.h"

// 改进：更清晰的委托参数命名
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawned, ABaseEnemy*, Enemy);

UCLASS()
class MYTOWERDEFENCE_API ASpawnEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEnemy")
	TSubclassOf<ABaseEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnEnemy")
	TArray<ABaseEnemy*> SpawnedEnemies;

	// 生成参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval; // 生成间隔（秒）

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxEnemiesToSpawn; // 最大生成数量

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	bool bAutoStartSpawning; // 是否自动开始生成

	// 改进：确保委托在蓝图中可见且可绑定
	UPROPERTY(BlueprintAssignable, Category = "Spawning")
	FOnEnemySpawned OnEnemySpawned;

protected:
	// 样条组件引用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Path")
	AActor* SplineActor;

	// 生成相关变量
	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	int32 EnemiesSpawnedCount;

	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	bool bIsSpawning;

	FTimerHandle SpawnTimerHandle;

public:
	// 开始生成敌人
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning();

	// 停止生成敌人
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StopSpawning();

	UFUNCTION(BlueprintCallable, Category = "SpawnEnemy")
	void SpawnEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 改进：添加生命周期管理
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};