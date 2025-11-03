// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "001_Enemy/BaseEnemy.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameMode.generated.h"

/**
 *
 */
UCLASS()
class MYTOWERDEFENCE_API ATowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATowerDefenceGameMode();

public:
	// 已生成敌人的数组，存储所有生成的敌人实例
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnedEnemies")
	TArray<ABaseEnemy*> SpawnedEnemies;

private:
	int32 Money;

public:
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetMoney() const;
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AddMoney(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	bool SpendMoney(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandleEnemyDeath(ABaseEnemy* DeadEnemy);
};