// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"
//Dependent of MovementComponent through pathfinding

// Forward declarations
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:
	float AcceptanceRadius = 3000.f; // TODO: check if correct range (cm?)

	UTankAimingComponent* AimingComponent = nullptr;
	
};
