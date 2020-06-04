// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"	// this needs to be the last #include

// Forward Declaration instead of #include "Tank.h"
class ATank;
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATank* GetControlledTank() const;

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
