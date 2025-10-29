// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathPoint.generated.h"

UCLASS()
class MYTOWERDEFENCE_API APathPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathPoint")
	float LocationRadius = 200.0f;

	FVector TargetLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "TargetLocation")
	FVector GetTargetLocation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
