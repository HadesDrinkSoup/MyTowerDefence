// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SplineComponent.h>
#include "BaseEnemy.generated.h"

// 声明路径初始化完成的委托
// 当敌人路径初始化完成时广播，传递生成的路径组件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyPathInitialized, USplineComponent*, EnemyPath);

UCLASS()
class MYTOWERDEFENCE_API ABaseEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    // 构造函数
    ABaseEnemy();

public:
    // 敌人生命值属性，可在编辑器和蓝图中读写
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
    float Health;

    // 敌人移动路径组件，在编辑器中可见并在蓝图中可读写
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Path")
    USplineComponent* EnemyPath;

    // 路径初始化完成委托
    // 当敌人路径初始化完成后会广播此委托
    UPROPERTY(BlueprintAssignable, Category = "Enemy Events")
    FOnEnemyPathInitialized OnEnemyPathInitialized;

protected:
    // 游戏开始时调用的函数
    virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    // 每帧调用的更新函数
    virtual void Tick(float DeltaTime) override;

    // 设置玩家输入组件的函数
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    // 初始化敌人路径并触发委托
    // @param SplineActor - 包含样条组件的Actor引用
    // @return 返回初始化的样条组件
    UFUNCTION(BlueprintCallable, Category = "Enemy Path")
    USplineComponent* InitializeEnemyPath(AActor* SplineActor);
};