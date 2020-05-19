// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::RotateWithRelativeSpeed(float RotationSpeed)
{
    // Clamp the Rotation Speed
    RotationSpeed = FMath::Clamp<float>(RotationSpeed, -1, +1);

    // Rotation Change per second (Framerate independent)
    float RotationChange = RotationSpeed * MaxRotationSpeed * GetWorld()->DeltaTimeSeconds;

    // Calculate the new Yaw of the Turret
    float TurretRotation = RelativeRotation.Yaw + RotationChange;

    // Clamping of the Rotation not needed, as Turret is free to rotate 360°

    // Set the Turret-Rotator according to new Yaw
    SetRelativeRotation(FRotator(0, TurretRotation, 0));

    // Add the Rotation Change to the current Rotation
    // AddRelativeRotation(FRotator(0, RawRotationChange, 0));
}