// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
    CheckForPosessedTankAndPlayerTank();
}

void ATankAIController::Tick( float DeltaSeconds )
{
    Super::Tick ( DeltaSeconds );

    // Get the Tank the AI is posessing and Get the Tank the Player is posessing
    PosessedTank = Cast<ATank>(GetPawn());
    PlayerTank = GetPlayerTank();    

    // only if Posessed- and Player-Tank are found:
    if (PosessedTank && PlayerTank)
    {
        // Aim at the Player Tank location
        PosessedTank->AimAt(PlayerTank->GetActorLocation());

        // Fire
        PosessedTank->Fire();
    } 
}

ATank* ATankAIController::GetPlayerTank() const
{
    // Get the Pawn of the First Player
    APawn* FirstPlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    // Check if Pawn was found before Casting (If no pawn found, give Error message and return nullptr)
    if (!FirstPlayerPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("TankAIController, GetPlayerTank(), FirstPlayerPawn could not be found!"));
        return nullptr;
    }

    // Check if PlayerPawn is a Tank (If not, give Error message and return nullptr)
    ATank* FirstPlayerTank = Cast<ATank>(FirstPlayerPawn);
    if (!FirstPlayerTank)
    {
        UE_LOG(LogTemp, Error, TEXT("TankAIController, GetPlayerTank(), FirstPlayerController and Pawn found, but it is not controlling a Tank!"));
        return nullptr;
    }

    return FirstPlayerTank;
}

void ATankAIController::CheckForPosessedTankAndPlayerTank()
{
    // Check if both AI Tank and PlayerTank are there and found
    if (! (Cast<ATank>(GetPawn()) && Cast<ATank>(GetPawn())) )
    {
        UE_LOG(LogTemp, Error, TEXT("TankAIContoller-hc hasn't found a tank to posess or hasn't found the PlayerTank. Protection from null ptr."));
    }
}