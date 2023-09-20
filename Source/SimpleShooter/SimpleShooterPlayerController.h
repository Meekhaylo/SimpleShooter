// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// For this file to be able to run by unreal engine, its module has to be included into build file:
// In this case name of the file == SimpleShooter.Build.cs
// PublicDependencyModuleNames.AddRange(new string[] { "UMG" });

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimpleShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASimpleShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float RestartDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrosshairClass;
	
	UPROPERTY() // For Garbage collector purposes
	UUserWidget* Crosshair;

	FTimerHandle RestartDelayTimer;

};
