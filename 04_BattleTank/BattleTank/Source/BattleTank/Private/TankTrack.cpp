// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

// Constructor (manually added)
UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = true;
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

    // Get the sideways speed with which the tracks are slipping and divide by two to get the tanks sideway speed
    float SidewaySpeed = FVector::DotProduct( GetRightVector(), GetComponentVelocity() ) / 2;

    // Calculate the opposite needed acceleration per frame v = at --> a = v/t
    float NeededAccelerationPerFrame = - SidewaySpeed/DeltaTime;

    // Calculate the force to be applied (F = ma) as a vector
    auto TankRoot = Cast<UPrimitiveComponent> (GetOwner()->GetRootComponent() );
    // Calculate the force to be applied (F = ma) as a vector
    FVector SidewayForce = (TankRoot->GetMass() * NeededAccelerationPerFrame) * GetRightVector();

    // Add Sideway Force to the Tank
    TankRoot->AddForce(SidewayForce);
}

// UPrimitiveComponent* OnComponentHit,
void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("Track is hitting"));
}

void UTankTrack::SetThrottle(float Throttle)
{
    // UE_LOG(LogTemp, Warning, TEXT("%s Throttle is: %f"), *GetName(), Throttle );

    // TODO: Clamp the values, in case we give the player the possibility to change the sensitivity of controls

    // GetOwner() gives an AActor*, Root Component will be a USceneComponent*, but we need at least a UPrimitveComponent type, so we cast it and save it to Tankroot
    auto TankRoot = Cast<UPrimitiveComponent>( GetOwner()->GetRootComponent() );
    FVector ForceLocation = GetComponentLocation();
    FVector ForceApplied = Throttle * TrackMaxDrivingForce * GetForwardVector();

    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}