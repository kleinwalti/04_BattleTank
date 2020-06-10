// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming / Firing State
UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

// Forward Declaration
class UTankBarrel;
class AProjectile;
class UTankTurret;

// Is handling the Tank Aiming
UCLASS( ClassGroup=(TankCustom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Function called by TankPlayerController for aiming
	void AimAt(FVector HitLocation);

	// Sets the Reference for the Barrel and the Turret
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitializeAimingReference(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	// Makes it possible to set the Projectile_BP in Blueprint for this variable 'ProjectileBlueprint' to be used in C++
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint = nullptr;

	// Fire Projectile, called by Input event in Tank_BP Input Setup
	UFUNCTION(BlueprintCallable)
	void Fire();

	// Used for the AI Tanks so they only fire when locked
	EFiringState GetFiringState();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	int32 GetRoundsLeft();
protected:
	// Called when the game starts.
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	EFiringState FiringState = EFiringState::Reloading;


	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 RoundsLeft = 20;
	

private:
	// define what a Barrel is. In this case, we define the Barrel to be a UStaticMeshComponent and we set it with our
	// SetBarrelReference function, which is called in blueprint and in blueprint we pass in the getBarrel reference.
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	void MoveBarrelTowards();	// ( FVector AimDirection )

	bool IsBarrelMoving();

	// needed to be a member variable to be used in the IsBarrelMoving() function.
	FVector AimDirection;

	// Launch Speed of the Projectile, used by the AimAt() function
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000.f; // TODO: Change this to a sensible starting value

	// Time needed to Reload a Projectile
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 2.f;

	// Initialize LastFireTime to 0
	double LastFireTime = 0;

};