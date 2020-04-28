// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"	// this needs to be the last #include

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	ATank* GetControlledTank() const;
	void AimTowardsCrosshair() const;
	bool GetSightRayHitLocation(FVector &HitLocation) const;
	bool GetLookVectorHitLocation(FVector &HitLocation, FVector CameraWorldLocation, FVector LookVector) const;

	UPROPERTY (EDITANYWHERE)
	float CrosshairXLocation = 0.5f;

	UPROPERTY (EDITANYWHERE)
	float CrosshairYLocation = 0.3333f;
};
