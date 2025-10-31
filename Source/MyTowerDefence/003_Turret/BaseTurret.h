// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "BaseTurret.generated.h"

USTRUCT(BlueprintType)
struct FTurretData : public FTableRowBase  // 修正拼写错误：FTurretDate -> FTurretData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* GroundEffect;
};

UCLASS()
class MYTOWERDEFENCE_API ABaseTurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseTurret();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	int32 UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	float AttackSpeed;

	// 炮塔网格体组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TurretMesh")
	class UStaticMeshComponent* TurretMesh;

	// 地面效果 Niagara 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TurretEffect")
	class UNiagaraComponent* GroundEffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	class UDataTable* TurretDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	FName TurretRowName;  // 更明确的命名

private:
	FTurretData* LoadedTurretData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "TurretData")
	bool InitializeTurretFromDataTable();
};