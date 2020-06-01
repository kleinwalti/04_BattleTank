// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankMovementComponent.h"
#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Creates the C++ Class UTankAimingComponent(inherited from UActorComponent) named "Aiming Component" in the Tank_BP ..
	// .. and assign it to the variable 'TankAimingComponent'
	// TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(TEXT("Aiming Component"));	// Remove, because not needed anymore. Is now added manually
	// TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(TEXT("Movement Component"));	// TODO: Remove, because not needed anymore
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

// TODO: We don't know what a TankAimingComponent is at the moment.
//	PlayerController could either tell TankAimingComponent directly to aim or we somehow pass a TankAimingComponent back to the Tank
void ATank::AimAt(FVector HitLocation)
{
	// Protection from null-ptr (now needed, because TankAimingComponent not created in constructer of tank.cpp)
	if (!TankAimingComponent) { UE_LOG(LogTemp, Error, TEXT("No TankAimingComponent found")); return; }
	// Delegating the aiming to the TankAimingComponent, which is from type UTankAimingComponent and the variable is assigned at begin play.
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

// Move this function to Tank Aiming Component and pass Projectile towards TAC
// Then only call this function here to be called in the TAC
void ATank::Fire()
{
	// Protection from nullptr (maybe not needed for ProjectileBlueprint Variable)
	if(!Barrel) {UE_LOG(LogTemp, Error, TEXT("ATank::Fire() - No Barrel found.")); return;}
	if(!ProjectileBlueprint) {UE_LOG(LogTemp, Error, TEXT("Projectile not set in Tank_BP.")); return; }

	bool bIsReloaded = ( FPlatformTime::Seconds() - LastFireTime ) > ReloadTimeInSeconds;

	if (bIsReloaded)
	{
		// Get the Socket Location and Rotation
		FVector SocketLocation = Barrel->GetSocketLocation( FName(TEXT("BarrelEnd")) );
		FRotator SocketRotation = Barrel->GetSocketRotation( FName(TEXT("BarrelEnd")) );

		// Spawn Projectile_BP (set in Tank_BP) at Socket Location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, SocketLocation, SocketRotation);
		Projectile->LaunchProjectile(LaunchSpeed);

		// Set the last fire time to current time
		LastFireTime = FPlatformTime::Seconds();
	}
}

void ATank::SetTankAimingComponentReferenceForTankCpp(UTankAimingComponent* TankAimingComponentToSet, UTankBarrel* BarrelToSet)
{
	// Protection
	if (!TankAimingComponentToSet) {UE_LOG(LogTemp, Error, TEXT("%s reports: NoTankAimingComponentToSet! Check Tank_BP BeginPlay maybe?"), *GetOwner()->GetName());}
	
	// TankAimingComponentToSet is passed in via blueprint
	TankAimingComponent = TankAimingComponentToSet;

	// Just to check if it works: Give the Tank class also a reference to what a barrel is:
		// TODO: If I keep this inside, change name of function!!!
	Barrel = BarrelToSet;
}