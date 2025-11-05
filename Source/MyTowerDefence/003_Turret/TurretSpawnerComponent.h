// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "002_GameMode/TowerDefenceGameMode.h"
#include "003_Turret/TurretDataTableManager.h"
#include "003_Turret/BaseTurret.h"
#include "Engine/DataTable.h"
#include "TurretSpawnerComponent.generated.h"

class ABaseTurret;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTOWERDEFENCE_API UTurretSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTurretSpawnerComponent();

public:
	// 主类型数据表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Spawner")
	UDataTable* TurretTypeDataTable;

	// 当前选择的类型
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret Spawner")
	FTurretType SelectedType;

	// 当前选择的子类数据
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret Spawner")
	FBaseTurretData SelectedSubclassData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret Spawner")
	FName SelectedSubclassRowName;

	// 所有可用的炮塔类型名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Spawner")
	TArray<FName> AvailableTurretTypes;

	// 所有可用的炮塔子类名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Spawner")
	TArray<FName> AvailableSubclassRows;

	// 最近生成的炮塔引用
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Spawner")
	ABaseTurret* LastSpawnedTurret;

	// 生成偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Spawner")
	FVector SpawnOffset = FVector(0, 0, 100);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	ATowerDefenceGameMode* GameModeRef;
	UTurretDataTableManager* DataTableManager;

	// 延迟初始化
	void DelayedInitialize();
	FTimerHandle InitializeTimerHandle;

	// 引用获取方法
	ATowerDefenceGameMode* GetGameModeRef() const;
	UTurretDataTableManager* GetDataTableManager() const;
	bool EnsureInitialized() const;

public:
	// 类型选择函数
	UFUNCTION(BlueprintCallable, Category = "Turret Spawner")
	bool SelectTurretType(FName TypeRowName);

	UFUNCTION(BlueprintCallable, Category = "Turret Spawner")
	bool SelectTurretSubclass(FName SubclassRowName);

	// 生成炮塔函数
	UFUNCTION(BlueprintCallable, Category = "Turret Spawner")
	ABaseTurret* SpawnTurret(FName TypeRowName, FName SubclassRowName);

	// 检查是否可以生成炮塔
	UFUNCTION(BlueprintCallable, Category = "Turret Spawner")
	bool CanSpawnTurret() const;

	// 获取生成成本
	UFUNCTION(BlueprintCallable, Category = "Turret Spawner")
	int32 GetSpawnCost() const;
};