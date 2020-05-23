// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Creates the C++ Class UTankAimingComponent(inherited from UActorComponent) named "Aiming Component" in the Tank_BP ..
	// .. and assign it to the variable 'TankAimingComponent'
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(TEXT("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::AimAt(FVector HitLocation)
{
	// Delegating the aiming to the TankAimingComponent, which is from type UTankAimingComponent and the variable is assigned at begin play.
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

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

// This function is called by the Tank_BP
void ATank::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	if (!BarrelToSet) {UE_LOG(LogTemp, Error, TEXT("%s reports: Tank.cpp-SetBarrelReference: BarrelToSet is not set! Check Tank_BP BeginPlay maybe?"), *GetOwner()->GetName());}
	// .. and when the Barrel is set in BP then pass it to the SetBarrelReference function in the TankAimingComponent
	TankAimingComponent->SetBarrelReference(BarrelToSet);


	// But also keep a local Barrel reference in the ATank class.
	Barrel = BarrelToSet;

	// so this is what I think is happening:
	// We setup the function in the Tank.h to be called by blueprint and we call this function via blueprint in the Tank_BP.
	// Because this function needs the UStaticMeshComponent* BarrelToSet to be passed into, we pass the Barrel-Component into the function, which is a UStaticMeshComponent

	// The function in the Tank.cpp now has the Barrel as the BarrelToSet passed into the fcuntion.
	// All it does now is to call the SetBarrelReference function in the TankAimingComponent and passing th BarrelToSet (which is the Barrel Component)

	// All the SetBarrelReference function in the TankAimingComponent does is setting the Barrel (which is defined in the TankAimingComponent) to be the BarrelToSet (which is the actual Barrel-Component)

	// So by all this we achieved the variable Barrel in the TankAimingComponent to be our actual Barrel-Component
}

void ATank::SetTurretReference(UTankTurret* TurretToSet)
{
	if (!TurretToSet) {UE_LOG(LogTemp, Error, TEXT("%s reports: Tank.cpp-SetTurretReference: TurretToSet is not set! Check Tank_BP BeginPlay maybe?"), *GetOwner()->GetName());}

	TankAimingComponent->SetTurretReference(TurretToSet);
}