// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	FString TankName = GetOwner()->GetName();
	// FVector BarrelLocation = Barrel->GetComponentLocation();
	// UE_LOG(LogTemp, Warning, TEXT("%s is aiming. From: %s towards: %s"), *TankName, *BarrelLocation.ToString(), *HitLocation.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("%s reports: My LaunchSpeed is: %f"), *TankName, LaunchSpeed);

	if ( !Barrel ) { UE_LOG(LogTemp, Error, TEXT("ERROR in AimAt()-UTankAimingComponent")); return; }
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
		)	// TODO: set draw debug line to false
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