// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "TankMovementComponent.h"

// Get Reference to left and right track. This function is called from blueprint.
void UTankMovementComponent::InitializeTracks(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
    // Pointer Protection
    if ( !ensure(LeftTrackToSet) || !ensure(RightTrackToSet)) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track to Set! nullptr prot.")); return; }   // actually not really needed, but hey, some info if something is not right
    
    // Setting the Variables to be the tracks passed in via blueprint
    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

// Movement of AI Tanks using the Pathfinding logic
void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    // Get the local x-Axis of the Tank (i.e. the forward axis)
    FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal(); // Get Safe Normal () not needed here, but it makes it really sure to have a unit vector
    
    // Get the wanted velocity, which the pathfinding calculates in order to get to the Actor defined in the MoveToActor() function, called in the TankAIController (in this case the player)
    FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

    // calculate difference in angles of Movement Intention and Tank Alignment and calculate values for rotation and force in direction of tank, using cross and dot product
    float ForwardMovement = FVector::DotProduct(TankForward, AIForwardIntention);
    float RightRotation = FVector::CrossProduct(TankForward, AIForwardIntention).Z;

    // Giving these values to the Movement and Rotation function, so that also the AI Tank can move and rotate (same way as player does)
    IntendMoveForward(ForwardMovement);
    IntendTurnRight(RightRotation);
}

// Forward Movement function
void UTankMovementComponent::IntendMoveForward(float Throw)
{
    // Pointer Protection
    if ( !ensure(LeftTrack || RightTrack)) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track! nullptr prot.")); return; }

    // Add a force to both tracks simultaneous (by calling SetThrottle() )
    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);

    // UE_LOG(LogTemp, Error, TEXT("IntendMoveForward is being called"));
}

// Rotation function
void UTankMovementComponent::IntendTurnRight(float Throw)
{
    // Pointer Protection
    if ( !ensure(LeftTrack || RightTrack)) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track! nullptr prot.")); return; }

    // Add opposing forces to left and right track (by calling SetThrottle() )
    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(-Throw);

    // UE_LOG(LogTemp, Error, TEXT("IntendTurnRight is being called"));
}