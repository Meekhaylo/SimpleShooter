// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASimpleShooterAIController::BeginPlay()
{
    Super::BeginPlay();
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
        
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        //GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    }
}

void ASimpleShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    

    /*if (this->LineOfSightTo(PlayerPawn, FVector(0, 0, 0)))
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    }
    else
    {
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }*/
}

bool ASimpleShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledChar = Cast<AShooterCharacter>(GetPawn());
    if (ControlledChar != nullptr)
    {
        return ControlledChar->IsAlive();
    }

    return true;
}
