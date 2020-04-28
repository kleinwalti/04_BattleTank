// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    ATank* PosessedTank = GetControlledTank();
    
    if(PosessedTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("The tank, posessed by TankPlayerController is: %s"), *PosessedTank->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NO PLAYER TANK FOUND! - TankPlayerController.cpp"));
    }
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    // UE_LOG(LogTemp, Warning, TEXT("Sie ticken ja. Boom."));
    AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}


void ATankPlayerController::AimTowardsCrosshair() const
{
    FVector HitLocation;

    if (!GetControlledTank()) { UE_LOG(LogTemp, Error, TEXT("No Tank is controlled by TankPlayerController - AimTowardsCrosshair()")); return; }

    if(GetSightRayHitLocation(HitLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Location is: %s"), *HitLocation.ToString());
    }

}
bool ATankPlayerController::GetSightRayHitLocation(FVector &HitLocation) const
{
    // Get ScreenLocation of the Crosshair Dot
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenLocation = {ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation};

    // Get Look Vector (UnitVector from that Crosshair dot into the world)
    FVector CameraWorldLocation, LookVector;  // WorldLocation is CamerWorldLocation, is just needed for function, but not otherwise used
    if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookVector))
    {
        GetLookVectorHitLocation(HitLocation, CameraWorldLocation, LookVector);
    }
    return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector &HitLocation, FVector CameraWorldLocation, FVector LookVector) const
{
    FHitResult HitResult;
    float Range = 1000000.f;

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