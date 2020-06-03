// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
// #include "TankAimingComponent.h"
#include "Tank.generated.h"

class AProjectile;
class UTankBarrel;
class UTankTurret;
class UTankAimingComponent;
class UTankMovementComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint = nullptr;

	// Give a reference to the TankAimingComponent via Blueprint, so the PlayerController can call the Aim() function on the tank and doesn't have to call any subclasses
	// This is just my personal code
	// UFUNCTION(BlueprintCallable, Category = "Setup")
	// void SetTankAimingComponentReferenceForTankCpp(UTankAimingComponent* TankAimingComponentToSet, UTankBarrel* BarrelToSet);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY (BlueprintReadOnly)
	UTankAimingComponent* TankAimingComponent = nullptr;

	// TODO: Check if I even need this one and delete if not
	UPROPERTY (BlueprintReadOnly)	
	UTankMovementComponent* TankMovementComponent = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000.f; // TODO: Change this to a sensible starting value

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3.f;

	// creating local reference to Barrel in ATank class.
	UTankBarrel* Barrel = nullptr;

	double LastFireTime = 0;
};
