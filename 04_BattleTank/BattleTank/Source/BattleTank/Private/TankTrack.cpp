// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

// Constructor (manually added)
UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// BeginPlay (manually added)
void UTankTrack::BeginPlay()
{
    Super::BeginPlay();

    // Register delegate at begin play like this:
    OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

// Tick (manually added)
void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// UPrimitiveComponent* OnComponentHit,
void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // UE_LOG(LogTemp, Warning, TEXT("Track is hitting"));

    // Only when on ground ..
    // .. set the Movement force
    DriveTrack();

    // .. apply force to stop sliding sideways
    ApplySidewaysForce();

    // Reset the Throttle to zero (until new throw / throttle comes throught the input binding)
    CurrentThrottle = 0;
}

// CLARIFY!
void UTankTrack::SetThrottle(float Throttle)
{
    // UE_LOG(LogTemp, Warning, TEXT("%s: %s Throttle is: %f"), *GetOwner()->GetName(), *GetName(), Throttle );

    // TODO: Clamp the values, in case we give the player the possibility to change the sensitivity of controls
    // We need this, because SetThrottle() is called by more than one event (Tank_BP) or functions (MovementComponent)
        // and sometimes it is called with a throttle value of 0 which would reset the currentthrottle
        // but in this case, if any time the currentThrottle has a value geater than zero, it is maintained until it is reset AFTER the DriveTrack() function is called
    CurrentThrottle = FMath::Clamp<float>((CurrentThrottle + Throttle), -1, +1);
    // UE_LOG(LogTemp, Warning, TEXT("%s: %s CurrentThrottle is: %f"), *GetOwner()->GetName(), *GetName(), CurrentThrottle );
    // CurrentThrottle = Throttle;


    // CLARIFY!!!
    // Why is this function called every frame and not only on input event?????????????????????
}

void UTankTrack::DriveTrack()
{
    // CurrentThrottle is correctly set in the SetThrottle() function by the TankMovement Component.
    // But when retrieving the value of CurrentThrottle in the DriveTrack() function, the CurrentThrottle was already set to zero again by the
    // Throttle Left/Right Track event in the Tank_BP which calls the SetThrottle() function and as there was no input on that event
    // it is now retrieved as zero.
    // I think because the way of the Throttle Left/Right Track is somewhat shorter or has a better position in the race condition, it seems to work
    // Get Force Location and Force Vector
    FVector ForceLocation = GetComponentLocation();
    FVector ForceApplied = CurrentThrottle * TrackMaxDrivingForce * GetForwardVector();

    // UE_LOG(LogTemp, Warning, TEXT("%s: %s ForceToBeAdded is: %s"), *GetOwner()->GetName(), *GetName(), *ForceApplied.ToString() );

    // Add the Force at the Track-ComponentLocation
        // GetOwner() gives an AActor*, Root Component will be a USceneComponent*, but we need at least a UPrimitveComponent type, so we cast it and save it to Tankroot
    auto TankRoot = Cast<UPrimitiveComponent>( GetOwner()->GetRootComponent() );
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

    // CurrentThrottle = 0;
}

void UTankTrack::ApplySidewaysForce()
{
    // Get the sideways speed with which the tracks are slipping and divide by two to get the tanks sideway speed
    float SidewaySpeed = FVector::DotProduct( GetRightVector(), GetComponentVelocity() ) / 2;

    // Calculate the opposite needed acceleration per frame v = at --> a = v/t
    float DeltaTime = GetWorld()->GetDeltaSeconds();
    float NeededAccelerationPerFrame = - SidewaySpeed/DeltaTime;

    // Calculate the force to be applied (F = ma) as a vector
    auto TankRoot = Cast<UPrimitiveComponent> (GetOwner()->GetRootComponent() );
    FVector SidewayForce = (TankRoot->GetMass() * NeededAccelerationPerFrame) * GetRightVector();

    // Add Sideway Force to the Tank
    TankRoot->AddForce(SidewayForce);

    // CurrentThrottle = 0;
}