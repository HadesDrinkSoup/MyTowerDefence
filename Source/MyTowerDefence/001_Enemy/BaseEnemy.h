// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SplineComponent.h>
#include "BaseEnemy.generated.h"

/**
 * 塔防游戏中的敌人基类
 * 负责沿样条路径移动，包含生命值、速度等敌人属性
 * 继承自ACharacter，可以使用角色相关的移动和动画功能
 */
UCLASS()
class MYTOWERDEFENCE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// 构造函数：设置默认属性
	ABaseEnemy();

public:
	// 敌人属性
	// 生命值，当降至0或以下时敌人被消灭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float Health;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Path")
	USplineComponent* EnemyPath;

protected:
	// 游戏开始或生成时调用
	virtual void BeginPlay() override;

public:
	// 每帧调用
	virtual void Tick(float DeltaTime) override;

	// 绑定功能到输入
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 获取样条组件的蓝图可调用函数
	UFUNCTION(BlueprintCallable, Category = "Enemy Path")
	USplineComponent* InitializeEnemyPath(AActor* SplineActor);
	
};