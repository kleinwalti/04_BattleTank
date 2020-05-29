// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 Is responsible for the movement over ground of the Tank
 */
UCLASS( ClassGroup=(TankCustom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION ( BlueprintCallable, Category = Setup)
	void InitializeTracks(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet);

	UFUNCTION ( BlueprintCallable, Category = Input)
	void IntendMoveForward(float Throw);

	UFUNCTION ( BlueprintCallable, Category = Input)
	void IntendTurnRight(float Throw);

private:
	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;
};
