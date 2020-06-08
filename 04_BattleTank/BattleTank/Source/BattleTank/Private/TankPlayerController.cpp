// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "TankAimingComponent.h"
#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Get the Aiming component and assign it to the variable. If it exists, call the event, if not, create a log
    UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) { return; }
    FoundTankAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Aim to Crosshair every frame
    AimTowardsCrosshair();
}

// Call the other two functions and then delegate aiming TankAimingComponent by passing the HitLocation
void ATankPlayerController::AimTowardsCrosshair() const
{
    // Define variable Hit Location
    FVector HitLocation;

    // Find again the AimingComponent (in case we died, we will posess a different pawn now)
    UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

    // Protect the UTankAimingComponent pointer
    if (!ensure(AimingComponent)) { return; }

    // Get the HitLocation, and when we have it ..
    if(GetSightRayHitLocation(HitLocation))
    {
        // UE_LOG(LogTemp, Warning, TEXT("YES HitLocation"));
        // .. , but delegate the actual aiming to the TankAimingComponent
        AimingComponent->AimAt(HitLocation);
    }
    else
    {
        // UE_LOG(LogTemp, Warning, TEXT("NO HitLocation"));
    }
}

// Get the x-hair screen postion and the camera world position and pass it into the LineTracing function.
bool ATankPlayerController::GetSightRayHitLocation(FVector &HitLocation) const
{
    // Get ScreenLocation of the Crosshair Dot
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenLocation = {ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation};

    // Get Look Vector (UnitVector from that Crosshair dot into the world)
    FVector CameraWorldLocation, LookVector;  // WorldLocation is CameraWorldLocation, is just needed for function, but not otherwise used
    if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookVector))
    {
        // Return if the LineTracing hit something or not
        return GetLookVectorHitLocation(HitLocation, CameraWorldLocation, LookVector);
    }
    return false;
}

// Do the LineTracing and return the HitLocation, if something was hit. Otherwise, return 0-Vector.
bool ATankPlayerController::GetLookVectorHitLocation(FVector &HitLocation, FVector CameraWorldLocation, FVector LookVector) const
{
    FHitResult HitResult;
    float Range = 1000000.f;    // Reichweite des LineTracings

    // LineTrace für alles, was sichtbar ist
    if( GetWorld()->LineTraceSingleByChannel(HitResult, CameraWorldLocation, LookVector * Range, ECollisionChannel::ECC_Visibility) )
    {
        HitLocation = HitResult.Location;
        return true;
    }
    else
    {
        HitLocation = FVector(0);
        return false;
    }
}