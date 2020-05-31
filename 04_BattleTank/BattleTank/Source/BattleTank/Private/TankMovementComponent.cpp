// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "TankMovementComponent.h"

void UTankMovementComponent::InitializeTracks(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
    if ( !LeftTrackToSet || !RightTrackToSet) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track to Set! nullptr prot.")); return; }   // actually not really needed, but hey, some info if something is not right
    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal(); // Get Safe Normal () not needed here, but it makes it really sure to have a unit vector
    FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

    float ForwardMovement = FVector::DotProduct(TankForward, AIForwardIntention);
    float RightRotation = FVector::CrossProduct(TankForward, AIForwardIntention).Z;

    // UE_LOG(LogTemp, Warning, TEXT("%s reports TankForward: %s, ForwardIntention: %s, ForwardMovement: %f"), *GetOwner()->GetName(), *TankForward.ToString(), *AIForwardIntention.ToString(), ForwardMovement );
    IntendMoveForward(ForwardMovement);
    IntendTurnRight(RightRotation);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    // UE_LOG(LogTemp, Warning, TEXT("%s Throw is: %f"), *GetName(), Throw );   // delete
    if ( !LeftTrack || !RightTrack) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track! nullptr prot.")); return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);

    // RequestDirectMove(MoveVelocity, true);

    // UE_LOG(LogTemp, Warning, TEXT("Something reports MoveVelocity of: %s"), *MoveVelocity.ToString() );
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
    if ( !LeftTrack || !RightTrack) {UE_LOG(LogTemp, Error, TEXT("No Left or No Right Track! nullptr prot.")); return; }

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(-Throw);
}