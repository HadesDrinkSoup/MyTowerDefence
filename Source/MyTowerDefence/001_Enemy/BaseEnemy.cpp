// Fill out your copyright notice in the Description page of Project Settings.

#include "001_Enemy/BaseEnemy.h"
#include "BaseEnemy.h"

// 构造函数：设置默认值
ABaseEnemy::ABaseEnemy()
{
	// 设置此角色每帧调用Tick()，如果不需要可以提高性能关闭
	PrimaryActorTick.bCanEverTick = true;

	// 初始化敌人属性
	Health = 100.0f; // 默认生命值100
}

// 游戏开始或生成时调用
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// 每帧调用
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 绑定功能到输入（敌人通常由AI控制，此函数可能为空）
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 敌人通常不需要玩家输入，因此保持空实现
}

USplineComponent* ABaseEnemy::InitializeEnemyPath(AActor* SplineActor)
{
    
	if (SplineActor)
	{
		EnemyPath = SplineActor->FindComponentByClass<USplineComponent>();
		return EnemyPath;
	}
	return nullptr;
}
