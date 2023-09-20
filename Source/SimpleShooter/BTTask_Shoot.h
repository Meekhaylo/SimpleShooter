// Fill out your copyright notice in the Description page of Project Settings.

// For this file to be able to run by unreal engine, its module has to be included into build file:
// In this case name of the file == SimpleShooter.Build.cs
// PublicDependencyModuleNames.AddRange(new string[] { "GameplayTasks" });

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Shoot();
	

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
