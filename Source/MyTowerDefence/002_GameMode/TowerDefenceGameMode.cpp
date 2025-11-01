// Fill out your copyright notice in the Description page of Project Settings.


#include "002_GameMode/TowerDefenceGameMode.h"

ATowerDefenceGameMode::ATowerDefenceGameMode()
{
	Money = 100; // 初始金钱数
}

int32 ATowerDefenceGameMode::GetMoney() const
{
    return Money;
}

void ATowerDefenceGameMode::AddMoney(int32 Amount)
{
    for(auto & Enemy : SpawnedEnemies)
    {
        if(Enemy)
        {
            if (Enemy->IsDead())
            {
				Money += Enemy->Cost;
            }  
        }
	}
}

bool ATowerDefenceGameMode::SpendMoney(int32 Amount)
{
    return false;
}
