// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
    UE_LOG(LogTemp, Warning, TEXT("%s Throttle is: %f"), *GetName(), Throttle );
}