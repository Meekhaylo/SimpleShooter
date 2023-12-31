// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* Pawn = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Pawn->PrimaryFire();
		

	return EBTNodeResult::Succeeded;
}
