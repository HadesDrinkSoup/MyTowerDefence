#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "BaseTurret.generated.h"

USTRUCT(BlueprintType)
struct FTurretData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SellCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Damage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> TurretMesh;  // 使用指针类型

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UNiagaraSystem*> GroundEffect;  // 使用指针类型
};

UCLASS()
class MYTOWERDEFENCE_API ABaseTurret : public AActor
{
	GENERATED_BODY()

public:
	ABaseTurret();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 炮塔属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 UpgradeCost;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	class UDataTable* TurretDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	FName TurretRowName;

public:
	// 炮塔功能函数
	UFUNCTION(BlueprintCallable, Category = "Turret")
	bool InitializeTurretFromDataTable();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	bool UpgradeTurret();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	int32 GetSellCost() const;

private:
	const FTurretData* LoadedTurretData;

	// 辅助函数
	bool ValidateDataTableIndex(int32 Index) const;
};