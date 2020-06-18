// Copyright Michael Waltersdorf.

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Tank.h"   // for delegate
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

// Extending the Functionallity of the SetPawn() function of APlayerController, so to subscribe to a broadcasting list
void ATankPlayerController::SetPawn(APawn* InPawn)
{
    // Super::SetPawn() means: Don't change the SetPawn() original function, only add functionallity
    Super::SetPawn(InPawn);

    // Initialize Player Tank
    ATank* PlayerTank = nullptr;

    // Make sure there is a pawn
    if (InPawn)
    {
        // Cast that Pawn to a Tank
        PlayerTank = Cast<ATank>(InPawn);

        // Make sure we have that tank
        if (!ensure(PlayerTank)) { return; }

        // Add ATankPlayerControllers member function OnPlayerDeath() to the broadcasting list of the tank class ( see also Tank.cpp )
        PlayerTank->OnTankDeathDelegate.AddUniqueDynamic(this, &ATankPlayerController::OnPlayerDeath);
    }
}

// Handles what will happen when the Player died
void ATankPlayerController::OnPlayerDeath()
{
    // Start spectating mode, as the only mode allowed.
    StartSpectatingOnly();
    // UE_LOG(LogTemp, Warning, TEXT("The Player tank should have died now."));
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
    APawn* PlayerPawn = GetPawn();
    // Make sure we are posessing a pawn
    if (!PlayerPawn) { return; }    // TODO: remove the ensure, just make if(!PlayerPawn)   Yes, this one is triggered! Yes.

    // Define variable Hit Location
    FVector HitLocation;

    // Find again the AimingComponent (in case we died, we will posess a different pawn now)
    UTankAimingComponent* AimingComponent = PlayerPawn->FindComponentByClass<UTankAimingComponent>();

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