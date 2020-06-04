// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"
//Dependent of MovementComponent through pathfinding

class ATank;	// forward declaration instead of #include "Tank.h"

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:

	void CheckForPosessedTankAndPlayerTank();
	ATank* PosessedTank = nullptr;
	ATank* PlayerTank = nullptr;
	ATank* GetPlayerTank() const;

	float AcceptanceRadius = 3000.f; // TODO: check if correct range (cm?)
	
};
