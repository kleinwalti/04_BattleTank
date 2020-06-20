// Copyight Michael Waltersdorf.

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
	// Get Reference to left and right track
	UFUNCTION ( BlueprintCallable, Category = "Setup")
	void InitializeMovementComponent(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet, UAudioComponent* FastDrivingSoundToSet, UAudioComponent* IdleDrivingSoundToSet);

	// Forward Movement Function
	UFUNCTION ( BlueprintCallable, Category = "Input")
	void IntendMoveForward(float Throw);

	// Rotation Function
	UFUNCTION ( BlueprintCallable, Category = "Input")
	void IntendTurnRight(float Throw);

private:
	// Variables for left and right track
	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;

	// This function is called from the pathfinding logic by the AI Controller.
		// The pathfinding logic is called by MoveToActor() in the Tank AI Controller
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
};
