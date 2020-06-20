// Copyight Michael Waltersdorf.

#include "TankTrack.h"
#include "Components/AudioComponent.h"

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

    // Every frame Check if tank is going up and then apply downwardforce to avoid this jumping bug
    CheckIfGoingUpAndApplyDownwardForce();
}

// TODO: Solve Problem here: After some time I cannot move my tank, because the OnHit function is not called (uncomment log for testing)
void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // UE_LOG(LogTemp, Warning, TEXT("%s: Track is hitting"), *GetOwner()->GetName());

    // ApplyDownwardForce(1.f);

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
    
    // Activate Fast Driving Sound when Throttle input
    if (!ensure(FastDrivingSound)) { return; }
    if (!ensure(IdleDrivingSound)) { return; }
    if (CurrentThrottle != 0)
    {   
        // Pause the Idle Audio
        IdleDrivingSound->SetPaused(true);
        // Un-Pause the Driving Audio
        FastDrivingSound->SetPaused(false); // Note: When the audio is finished playing (on Audio finished) it will be activated again in BP (Tank_BP Input Binding)
    }
    // SidewaySpeed used, because some track will always have a currentTHrottle of 0 (for example conventional steering or turning)
    else if (CurrentThrottle == 0 &&  FGenericPlatformMath::RoundToInt(SidewaySpeed) == 0)
    {
        // Pause the Driving Sound
        FastDrivingSound->SetPaused(true);

        // Play the Idle Sound
        IdleDrivingSound->SetPaused(false); // Note: When the audio is finished playing (on Audio finished) it will be activated again in BP (Tank_BP Input Binding)
    }

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
    SidewaySpeed = FVector::DotProduct( GetRightVector(), GetComponentVelocity() ) / 2;

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

// Method used to counteract the spontaneous jumping of the tank due to ground being uneven and tank being dragged
void UTankTrack::CheckIfGoingUpAndApplyDownwardForce()
{
    // Get the upwards Speed of the tank
    float UpwardSpeed = FVector::DotProduct( GetUpVector(), GetComponentVelocity() );

    // Calculate the opposite needed acceleration per frame
    float DeltaTime = GetWorld()->GetDeltaSeconds();
    float NeededAccelerationPerFrame = - UpwardSpeed/DeltaTime;

    // Calculate the Force to be applied as a vector, but with a factor to make it not that big
    auto TankRoot = Cast<UPrimitiveComponent> ( GetOwner()->GetRootComponent() );
    FVector DownwardForce = 1.f * (TankRoot->GetMass() * NeededAccelerationPerFrame) * GetUpVector();

    // Calculate the Downward force (hopefully -Z in World Space)
    // FVector DownwardForce = EnterMinusForUpwardForce * -8000000.f * GetUpVector();   // TODO: Maybe find or calculate the magnitude

    // Add the Downward Force to the tank only if it is downwards
    if (DownwardForce.Z <= -0.f)
    {
        TankRoot->AddForce(DownwardForce);
        UE_LOG(LogTemp, Warning, TEXT("Downward Force of: %s"), *DownwardForce.ToString());
    }
    /*
    else
    {
        TankRoot->AddForce(-0.01 * DownwardForce);
        UE_LOG(LogTemp, Warning, TEXT("Downward Force of: %s"), *DownwardForce.ToString());
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Doing nothing"));
    */
}

void UTankTrack::GetTrackSoundEffects(UAudioComponent* FastDrivingSoundToSet, UAudioComponent* IdleDrivingSoundToSet)
{
    // UE_LOG(LogTemp, Warning, TEXT("Yes, this function gets called"));
    FastDrivingSound = FastDrivingSoundToSet;
    IdleDrivingSound = IdleDrivingSoundToSet;
}