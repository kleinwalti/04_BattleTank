// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    // Get and set the AimingComponent. TODO: Maybe set to tiick function in case of spawning new tanks during play?
    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
}

void ATankAIController::Tick( float DeltaSeconds )
{
    Super::Tick ( DeltaSeconds );

    // Get the Pawn the Player is posessing
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    // Check if Player-Pawn and AimingComponent are found, if not return
    if (! ensure(PlayerPawn || AimingComponent) ) { return; }

    // Move towards Player Pawn location
    MoveToActor(PlayerPawn, AcceptanceRadius);

    // Aim at the Player Pawn location
    AimingComponent->AimAt(PlayerPawn->GetActorLocation());

    // Fire
    AimingComponent->Fire();
}