// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{

    Super::BeginPlay();

    ATank* PosessedTank = Cast<ATank>(GetPawn());
    ATank* PlayerTank = GetPlayerTank();

    if (PosessedTank && PlayerTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("TankAIContoller-hc is possessing: %s and has found %s to be the PlayerTank"), *PosessedTank->GetName(), *PlayerTank->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TankAIContoller-hc hasn't found a tank to posess or hasn't found the PlyerTank. Protection from null ptr."));
    }
}

ATank* ATankAIController::GetPlayerTank() const
{
    APawn* FirstPlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!FirstPlayerPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("TankAIController, GetPlayerTank(), FirstPlayerPawn could not be found!"));
        return nullptr;
    }

    ATank* FirstPlayerTank = Cast<ATank>(FirstPlayerPawn);
    if (!FirstPlayerTank)
    {
        UE_LOG(LogTemp, Error, TEXT("TankAIController, GetPlayerTank(), FirstPlayerController and Pawn found, but it is not controlling a Tank!"));
    }

    return FirstPlayerTank;
}