// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(TankComponents), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Constructor (manually added)
	UTankTrack();

	// Tick (manually added)
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// BeginPlay (manually added)
	virtual void BeginPlay() override;

	// Sets the Throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	// The Maximum Throttle in Newton (or some other wierd value) (for a Tank mass of 100 kg works 2 000 000 ok)
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TrackMaxDrivingForce = 2000000.f;	// TODO: Find sensible starting value?

private:
	// I think it needs to be a UFUNCTION, thats just how this works, although we do not use it in Blueprint
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ApplySidewaysForce();
	void DriveTrack();

	// Badly fix the jumping issue, instead of continuing the tutorial
	void CheckIfGoingUpAndApplyDownwardForce();

	// To set the throttle but limit it to never being greater than +/- 1
	float CurrentThrottle = 1;
};
