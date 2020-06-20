// Copyright Michael Waltersdorf.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

// This class handles the health and the death event of the tanks
UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Create Delegate Type
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);
	// Declare the Delegate with Type
	FDeathDelegate OnTankDeathDelegate;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Overriding the Take Damage function
	// This method will get called by the engine when something is in range that says that damage should be taken (like the projectile)
	// Leaves the responsibility of taking damage in the hands of the tank, it does not have to take damage
	virtual float TakeDamage( float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser ) override;

	// Make it int32 so we can go to exactly 0.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 CurrentHealth = 100;

	// Gives the Current Health in percent of the Starting Health. Use BlueprintPure, so nothing can change the health in BP. It's like const.
	UFUNCTION(BlueprintPure, Category = "Setup")
	float GetCurrentHealth() const;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UAudioComponent* FiringSound = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* TankMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UParticleSystemComponent* DeathExplosion = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UAudioComponent* ImpactSound = nullptr;

	void OnTankDeath();
};
