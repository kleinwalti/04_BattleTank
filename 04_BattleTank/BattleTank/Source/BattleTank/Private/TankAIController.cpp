// Copyright Michael Waltersdorf.

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Tank.h"   // needed for the delegate
#include "TankAimingComponent.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    // Get and set the AimingComponent. TODO: Maybe set to tick function in case of spawning new tanks during play?
    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
}

// Extending the Functionallity of the SetPawn() function of AController, so to subscribe to a broadcasting list
void ATankAIController::SetPawn(APawn* InPawn)
{
    // Call the super class so everything in this function from the parent class will still be executed, and we are just adding functionality instead of completely overriding
    Super::SetPawn(InPawn);

    ATank* PosessedTank = nullptr;
    // Only do when there is a pawn
    if (InPawn)
    {
        // We cast that pawn to a tank
        PosessedTank = Cast<ATank>(InPawn);
        
        // Protection before using Posessed Tank
        if (!ensure(PosessedTank)) { return; }

        // Posessed Tank is the one having the delegate function which we want to add this one
        PosessedTank->OnTankDeathDelegate.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);    
    }
}

void ATankAIController::Tick( float DeltaSeconds )
{
    Super::Tick ( DeltaSeconds );

    // Get the Pawn the Player is posessing
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    // TODO: Remove. It is just for double making sure

    // Check if Player-Pawn and AimingComponent are found, if not return
    if (!PlayerPawn || !AimingComponent) { return; }

    // Move towards Player Pawn location, used by TankMovementComponent
    MoveToActor(PlayerPawn, AcceptanceRadius);

    // Aim at the Player Pawn location
    AimingComponent->AimAt(PlayerPawn->GetActorLocation());

    // Fire only when locked
    if (AimingComponent->GetFiringState() == EFiringState::Locked)
    {
        AimingComponent->Fire();
    }
}

void ATankAIController::OnTankDeath()
{
    // Remove the AI Controller from the Tank
    GetPawn()->DetachFromControllerPendingDestroy();
    // UE_LOG(LogTemp, Warning, TEXT("OnTankDeath() is being executed - AI Tank should die"));
}