// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterGameMode_DeathMatch.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "SimpleShooterAIController.h"

void ASimpleShooterGameMode_DeathMatch::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		//PlayerController->GameHasEnded(nullptr, false);
		EndGame(false);
	}

	for (ASimpleShooterAIController* Controller : TActorRange<ASimpleShooterAIController>(GetWorld()))
	{
		if (!(Controller->IsDead()))
		{
			return;
		}
	}

	EndGame(true);
}

void ASimpleShooterGameMode_DeathMatch::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))	
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
