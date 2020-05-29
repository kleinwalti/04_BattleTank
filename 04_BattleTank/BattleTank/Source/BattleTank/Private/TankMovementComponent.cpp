// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "TankMovementComponent.h"

void UTankMovementComponent::InitializeTracks(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
    if ( !LeftTrackToSet || !RightTrackToSet) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track to Set! nullptr prot.")); return; }   // actually not really needed, but hey, some info if something is not right
    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    // UE_LOG(LogTemp, Warning, TEXT("%s Throw is: %f"), *GetName(), Throw );   // delete
    if ( !LeftTrack || !RightTrack) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track! nullptr prot.")); return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
    if ( !LeftTrack || !RightTrack) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track! nullptr prot.")); return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(-Throw);
}