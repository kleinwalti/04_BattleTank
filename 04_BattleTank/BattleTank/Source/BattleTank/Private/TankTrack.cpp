// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
    UE_LOG(LogTemp, Warning, TEXT("%s Throttle is: %f"), *GetName(), Throttle );

    // TODO: Clamp the values, in case we give the player the possibility to change the sensitivity of controls

    // GetOwner() gives an AActor*, Root Component will be a USceneComponent*, but we need at least a UPrimitveComponent type, so we cast it and save it to Tankroot
    auto TankRoot = Cast<UPrimitiveComponent>( GetOwner()->GetRootComponent() );
    FVector ForceLocation = GetComponentLocation();
    FVector ForceApplied = Throttle * TrackMaxDrivingForce * GetForwardVector();

    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}