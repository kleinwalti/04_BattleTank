// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set the last fire time to BeginPlay time initially, so you have to reload first before being able to fire
	LastFireTime = FPlatformTime::Seconds();
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ( ( FPlatformTime::Seconds() - LastFireTime ) > ReloadTimeInSeconds )
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Reloading;
	}
	
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	FString TankName = GetOwner()->GetName();
	// FVector BarrelLocation = Barrel->GetComponentLocation();
	// UE_LOG(LogTemp, Warning, TEXT("%s is aiming. From: %s towards: %s"), *TankName, *BarrelLocation.ToString(), *HitLocation.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("%s reports: My LaunchSpeed is: %f"), *TankName, LaunchSpeed);

	if ( !ensure(Barrel) ) { UE_LOG(LogTemp, Error, TEXT("ERROR in AimAt()-UTankAimingComponent")); return; }
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("BarrelEnd"));

	if ( UGameplayStatics::SuggestProjectileVelocity(
		this, 
		OutLaunchVelocity, 
		StartLocation, 
		HitLocation, 
		LaunchSpeed,
		false, 
		0, 
		0, 
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false
		)	// TODO: set draw debug line to false and remove standard parameters if not used
	)
	{
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		// UE_LOG(LogTemp, Warning, TEXT("%s reports: StartLocation: %s, HitLocation: %s, LaunchDirection: %s"), *TankName, *StartLocation.ToString(), *HitLocation.ToString(), *AimDirection.ToString());
		MoveBarrelTowards(AimDirection);

		auto Time = GetWorld()->GetTimeSeconds();
   		// UE_LOG(LogTemp, Warning, TEXT("%f: Elevate () is called."), Time);
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
   		 UE_LOG(LogTemp, Warning, TEXT("%f: No solution found. Maybe because too far away."), Time);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	auto Time = GetWorld()->GetTimeSeconds();
	FRotator BarrelRotation = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotation = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotation - BarrelRotation;
	// UE_LOG(LogTemp, Warning, TEXT("%s reports at %f"), *GetOwner()->GetName(), Time);
	// UE_LOG(LogTemp, Warning, TEXT("BarrelRotation is: %s"), *BarrelRotation.ToString());

	// UE_LOG(LogTemp, Warning, TEXT("AimAsRotation is: %s"), *AimAsRotation.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("DeltaRotator is: %s"), *DeltaRotator.ToString());
	// (UTankBarrel*)->Elevate(5);	// Why can't we do like this?
	// AND HOW CAN WE CALL THE FUNCTIONS OF OTHER CLASSES OUTSIDE OF BLUEPRINT? (Just in c++?)
	// So why are we making the TankBarrel class Blueprint Spawnable, just to pass it from blueprint intro cpp via the 
	// SetBarrelReference function and pass it further down to TankAimingComponent, instead of just using the cpp-class TankBarrel itself in cpp? 

	// TODO: Ask in forum why we are doing the blueprint workaround and in blueprint give it the class and why not calling the class directly in c++?
	// UE_LOG(LogTemp, Warning, TEXT("DeltaRotator.Pitch is: %f"), DeltaRotator.Pitch);
	// UE_LOG(LogTemp, Warning, TEXT("DeltaRotator.Yaw is: %f"), DeltaRotator.Yaw);
	Barrel->ElevateWithRelativeSpeed(DeltaRotator.Pitch);
	Turret->RotateWithRelativeSpeed(DeltaRotator.Yaw);
}

void UTankAimingComponent::InitializeAimingReference(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	// Protection from null-pointer
	if (!BarrelToSet || !TurretToSet) {UE_LOG(LogTemp, Error, TEXT("TankAimingComponent.cpp-InitializeAimingReference(): I did not receive any Barrel or Turret ToSet! Careful, null-pointer!")); return; }

	// Set the Barrel and the Turret Reference
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::Fire()
{
	// Protection of Barrel and ProjectileBlueprint
	if(!ensure(Barrel || ProjectileBlueprint)) { return; }

	// Set bIsReloaded true after ReloadTime
	// bool bIsReloaded = ( FPlatformTime::Seconds() - LastFireTime ) > ReloadTimeInSeconds;

	// When reloaded ..
	if (FiringState == EFiringState::Aiming)
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