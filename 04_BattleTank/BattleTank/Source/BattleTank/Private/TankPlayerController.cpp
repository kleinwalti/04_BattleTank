// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Tank.h"
#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    ATank* PosessedTank = GetControlledTank();
    
    if(PosessedTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("The tank, posessed by TankPlayerController is: %s"), *PosessedTank->GetName());  // can be deleted later
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NO PLAYER TANK FOUND! - TankPlayerController.cpp"));
    }
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AimTowardsCrosshair();
}

// Call the other two functions and then delegate aiming to the tank by passing the HitLocation
void ATankPlayerController::AimTowardsCrosshair() const
{
    FVector HitLocation;
    if (!GetControlledTank()) { UE_LOG(LogTemp, Error, TEXT("No Tank is controlled by TankPlayerController, therefore no aiming. - AimTowardsCrosshair()")); return; }

    // Get the HitLocation ..
    if(GetSightRayHitLocation(HitLocation))
    {
        // .. , but delegate the actual aiming to the Tank, and then to the TankAimingComponent
        GetControlledTank()->AimAt(HitLocation);
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
        GetLookVectorHitLocation(HitLocation, CameraWorldLocation, LookVector);
    }
    return true;
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

ATank* ATankPlayerController::GetControlledTank() const
{
    return Cast<ATank>( GetPawn() );
}