// Copyright Michael Waltersdorf.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"	// this needs to be the last #include

// Forward Declarations
class UTankAimingComponent;

// Responsible for calculating the aiming of the player and passing it on to the TankAimingComponent,
// and responsible for handling the death of the player
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPlayerDeath();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundTankAimingComponent(UTankAimingComponent* AimingComponentReference);

private:
	void AimTowardsCrosshair() const;
	bool GetSightRayHitLocation(FVector &HitLocation) const;
	bool GetLookVectorHitLocation(FVector &HitLocation, FVector CameraWorldLocation, FVector LookVector) const;

	UPROPERTY (EditDefaultsOnly)
	float CrosshairXLocation = 0.5f;

	UPROPERTY (EditDefaultsOnly)
	float CrosshairYLocation = 0.3333f;
};
