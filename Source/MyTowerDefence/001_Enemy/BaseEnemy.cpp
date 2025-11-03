#include "BaseEnemy.h"
#include "Components/SplineComponent.h"

// 构造函数：初始化默认值
ABaseEnemy::ABaseEnemy()
{
    // 设置此角色每帧调用Tick()
    PrimaryActorTick.bCanEverTick = true;

	EnemyDataTable = nullptr;

	EnemyRowName = NAME_None;

    // 初始化路径指针为空
    EnemyPath = nullptr;
    
	Health = 10.0f;
	bIsDead = false;
}

// 游戏开始时的初始化
void ABaseEnemy::BeginPlay()
{
    Super::BeginPlay();
    if (EnemyDataTable && !EnemyRowName.IsNone())
    {
        InitializeEnemyFromDataTable();
    }
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清空委托绑定，防止内存泄漏
	OnEnemyPathInitialized.Clear();
	OnEnemyDead.Clear();
    // 调用父类结束游戏处理
	Super::EndPlay(EndPlayReason);
}

// 每帧更新函数
// @param DeltaTime - 距离上一帧的时间间隔
void ABaseEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 设置玩家输入组件
// @param PlayerInputComponent - 玩家输入组件指针
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 初始化敌人移动路径
// @param SplineActor - 包含样条组件的Actor引用
// @return 成功返回样条组件指针，失败返回nullptr
USplineComponent* ABaseEnemy::InitializeEnemyPath(AActor* SplineActor)
{
    // 检查传入的Actor是否有效
    if (SplineActor)
    {
        // 从Actor中查找样条组件
        EnemyPath = SplineActor->FindComponentByClass<USplineComponent>();

        // 检查是否成功找到样条组件
        if (EnemyPath)
        {
            // 日志记录成功信息，显示样条点数
            UE_LOG(LogTemp, Log, TEXT("[成功] 敌人 %s: 路径 %s 成功初始化 "), *GetName(), *SplineActor->GetName());

            // 检查委托是否已绑定，如果是则广播路径初始化完成事件
            if (OnEnemyPathInitialized.IsBound())
            {
                OnEnemyPathInitialized.Broadcast(EnemyPath);
            }
        }
        else
        {
            // 日志记录错误：未找到样条组件
            UE_LOG(LogTemp, Error, TEXT("[错误] 敌人 %s: 未找到样条组件"), *GetName());
        }

        // 返回样条组件指针（可能为nullptr）
        return EnemyPath;
    }

    // 日志记录错误：传入的Actor为空
    UE_LOG(LogTemp, Error, TEXT("[错误] 敌人 %s: SplineActor为空"), *GetName());

    // 返回空指针表示初始化失败
    return nullptr;
}

bool ABaseEnemy::InitializeEnemyFromDataTable()
{
    if (!EnemyDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 敌人数据表为空"));
        return false;
    }
    if (EnemyRowName.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 敌人命名未设置"));
        return false;
    }
    LoadedEnemyData = EnemyDataTable->FindRow<FEnemyData>(EnemyRowName, TEXT("查找敌人数据"));

    if (!LoadedEnemyData)
    {
        UE_LOG(LogTemp, Error, TEXT("[错误] 未在数据表中找到敌人数据: %s"), *EnemyRowName.ToString());
        return false;
    }

    // 应用数据表格中的属性
    if (LoadedEnemyData->SkeletalMesh)
    {
        GetMesh()->SetSkeletalMesh(LoadedEnemyData->SkeletalMesh);
    }

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = LoadedEnemyData->MoveSpeed;
    }

    // 设置生命值
    Health = LoadedEnemyData->Health;

    Cost = LoadedEnemyData->Cost;

    // 设置动画蓝图
    if (LoadedEnemyData->AnimClass)
    {
        GetMesh()->SetAnimInstanceClass(LoadedEnemyData->AnimClass);
    }
    UE_LOG(LogTemp, Log, TEXT("[成功] 敌人已在从表格数据初始化 %s"), *EnemyRowName.ToString());
    return true;
}

void ABaseEnemy::TakeDamage(float DamageAmount)
{
    Health -= DamageAmount;
    if(Health <= 0.0f && !bIsDead)
    {
		UE_LOG(LogTemp, Log, TEXT("敌人 %s 已死亡"), *GetName());
        bIsDead = true;
        if(OnEnemyDead.IsBound())
        {
            OnEnemyDead.Broadcast(this);
        }
        if (GetCharacterMovement())
        {
            GetCharacterMovement()->StopMovementImmediately();
            GetCharacterMovement()->DisableMovement();
        }
		SetLifeSpan(2.0f); // 2秒后销毁敌人
	}
}

bool ABaseEnemy::GetIsDead()
{
    return bIsDead;
}
