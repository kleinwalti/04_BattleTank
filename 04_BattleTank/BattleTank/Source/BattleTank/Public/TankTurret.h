// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(TankComponents), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	void RotateWithRelativeSpeed(float RotationSpeed);	

private:
	// How fast the turret can rotate as a maximum, in degrees per second
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxRotationSpeed = 60.f;	// TODO: Change to sensible starting value
};