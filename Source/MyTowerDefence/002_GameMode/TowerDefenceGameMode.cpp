// Fill out your copyright notice in the Description page of Project Settings.


#include "002_GameMode/TowerDefenceGameMode.h"

ATowerDefenceGameMode::ATowerDefenceGameMode()
{
	Money = 100; // 初始金钱数
	TurretDataTableManager = CreateDefaultSubobject<UTurretDataTableManager>(TEXT("TurretDataTableManager"));
}

int32 ATowerDefenceGameMode::GetMoney() const
{
    return Money;
}

void ATowerDefenceGameMode::AddMoney(int32 Amount)
{
    Money += Amount;
	UE_LOG(LogTemp, Log, TEXT("增加金钱: %d, 总金钱: %d"), Amount, Money);
}

bool ATowerDefenceGameMode::SpendMoney(int32 Amount)
{
	if (Money >= Amount)
	{
		Money -= Amount;
		UE_LOG(LogTemp, Log, TEXT("花费金钱: %d, 剩余金钱: %d"), Amount, Money);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[警告] 金币 %d 不足 %d，无法升级"), Money, Amount);
	}
    return false;
}

void ATowerDefenceGameMode::HandleEnemyDeath(ABaseEnemy* DeadEnemy)
{
	AddMoney(DeadEnemy->Cost);
	SpawnedEnemies.Remove(DeadEnemy);
}
