// Fill out your copyright notice in the Description page of Project Settings.


#include "001_Enemy/PathPoint.h"

// Sets default values
APathPoint::APathPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathPoint::BeginPlay()
{
	Super::BeginPlay();
	TargetLocation = FVector::ZeroVector;
	
}

FVector APathPoint::GetTargetLocation()
{
	GetActorLocation();
	float TargetX = FMath::RandRange(GetActorLocation().X - LocationRadius, GetActorLocation().X + LocationRadius);
	float TargetY = FMath::RandRange(GetActorLocation().Y - LocationRadius, GetActorLocation().Y + LocationRadius);
	TargetLocation = FVector(TargetX, TargetY, GetActorLocation().Z);
	return TargetLocation;
}

// Called every frame
void APathPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

