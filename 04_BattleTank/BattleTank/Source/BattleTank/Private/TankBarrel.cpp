// Copyight Michael Waltersdorf.

#include "TankBarrel.h"

void UTankBarrel::ElevateWithRelativeSpeed(float RelativeSpeed)
{
    // Get Word-Time for Debug-Logs
    auto Time = GetWorld()->GetTimeSeconds();

    // Clamp the RelativeSpeed
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);

    // UE_LOG(LogTemp, Error, TEXT("Pitch: Clamped Relative speed is: %f"), RelativeSpeed);

    // Calculate Pitch-Change per Second (Framerate Independent)
    float ChangePerSecond = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;

    // Calculate the un-clamped Elevation-Change
    float RawElevationChange = RelativeRotation.Pitch + ChangePerSecond;

    // clamp the Elevation-Change to be in limits
    float ClampedElevationChange = FMath::Clamp<float>(RawElevationChange, MinElevationDegrees, MaxElevationDegrees);

    // Set the new Elevation of the Barrel
    SetRelativeRotation(FRotator(ClampedElevationChange, 0, 0));


    // UE_LOG(LogTemp, Error, TEXT("Pitch: ChangePerSecond is: %f"), ChangePerSecond);

    // Add Pitch-Change to Relative Rotation
    // AddRelativeRotation(FRotator(ChangePerSecond, 0, 0));


    // UE_LOG(LogTemp, Warning, TEXT("%f: ClampedElevationChange is: %f"), Time, ClampedElevationChange);


    // UE_LOG(LogTemp, Warning, TEXT("%f: Elevate () is called. Elevation Speed is: %f"), Time, RelativeSpeed);




    // how to know RelativeRotation is a Property of a scene component. Is there like a list for all the Properties of a class, which I can reference ?
}