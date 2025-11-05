// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "003_Turret/BaseTurret.h"
#include "003_Turret/TurretDataTableManager.h"
#include "Engine/DataTable.h"
#include "Turret_SingleTarget.generated.h"

/**
 *
 */

UCLASS()
class MYTOWERDEFENCE_API ATurret_SingleTarget : public ABaseTurret
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SingleTargetTurretData")
	class UDataTable* SingleTargetTurretDataTable;

private:
	FBaseTurretData* SingleTargetTurretData;

public:
	virtual bool InitializeTurretFromDataTable_Implementation(FName NewTurretName) override;
	virtual bool UpgradeTurret_Implementation() override;
	virtual void SellOut_Implementation() override;
};