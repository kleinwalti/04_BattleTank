// Copyright Michael Waltersdorf.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"
//Dependent of MovementComponent through pathfinding

// Forward declarations
class UTankAimingComponent;

// Handles AI Aiming, Moving, Firing and Death
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	// Overriding the function where the Controller Sets the pawn to posess or something; needed for delegates
	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnTankDeath();

protected:
	// Means how close the AI tanks wants to be to the player, in cm. Somehow doesn't work when editing in BP. Edit Value in cpp or while playing game.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 8000.f;

private:
	UTankAimingComponent* AimingComponent = nullptr;
	
};
