// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "002_GameMode/TowerDefenceGameMode.h"
#include "003_Turret/TurretDataTableManager.h"
#include "GameFramework/Actor.h"
#include "BaseTurret.generated.h"

class UTurretDataTableManager;
struct FBaseTurretData;

UCLASS(Abstract) // 标记为抽象类，不能直接实例化
class MYTOWERDEFENCE_API ABaseTurret : public AActor
{
	GENERATED_BODY()

public:
	ABaseTurret();

public:
	// 炮塔属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	FName TurretName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 MaxLevel;

	// 当前等级为数组索引，从0开始
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 SellCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	float AttackSpeed;

	// 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* GroundEffectComponent;

	// 数据表引用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretTypeData")
	class UDataTable* TurretTypeDataTable;

	// 炮塔类型名称（用于数据表查找）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretTypeData")
	FName TurretTypeName;

protected:
	// 核心引用 - 使用mutable以便在const方法中修改
	mutable ATowerDefenceGameMode* GameModeRef;
	mutable UTurretDataTableManager* TurretDataTableManager;

protected:
	virtual void BeginPlay() override;

	// 延迟初始化
	void DelayedInitialize();
	FTimerHandle InitializeTimerHandle;

	// 懒加载和验证方法
	ATowerDefenceGameMode* GetGameModeRef() const;
	UTurretDataTableManager* GetTurretDataTableManager() const;
	bool EnsureInitialized() const;
	bool ValidateCriticalRef() const;

public:
	virtual void Tick(float DeltaTime) override;

public:

	// 炮塔功能函数，由子类实现
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Turret")
	bool InitializeTurretFromDataTable(FName NewTurretName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Turret")
	bool UpgradeTurret();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Turret")
	void SellOut();

	// 添加默认实现声明
	virtual bool InitializeTurretFromDataTable_Implementation(FName NewTurretName);
	virtual bool UpgradeTurret_Implementation();
	virtual void SellOut_Implementation();

	// 调试函数
	UFUNCTION(BlueprintCallable, Category = "Turret|Debug")
	void DebugTurretSetup();
};