// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

// TODO: I think he deleted this one
// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ATank::TakeDamage( float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser )
{
	UE_LOG(LogTemp, Warning, TEXT("%s is taking damage! Damage Amount: %f"), *GetName(), DamageAmount);

	// Reduce health

	// Convert to integer, so we can go to exactly 0
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);

	int32 DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;

	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor shuld die"));
		// Use the delegate to broadcast (to the AIController and the Player Controller)
		OnTankDeathDelegate.Broadcast();
	}
	
	return DamageToApply;
}

float ATank::GetCurrentHealth() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}