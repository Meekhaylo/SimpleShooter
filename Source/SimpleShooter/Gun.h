// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/EngineTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"


UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
    USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, Category = "Effect")
    class UParticleSystem* ShootParticles;

	UPROPERTY(EditAnywhere, Category = "Effect")
    class UParticleSystem* HitParticles;

    UPROPERTY(EditAnywhere, Category = "Effect")
    class USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "C++ Properties")
	float MaxRange = 10000;

	UPROPERTY(EditAnywhere, Category = "C++ Properties")
	float Damage = 1;

	bool GunTrace(FHitResult& HitResult, FVector& ShotDirection);
	
	AController* GetOwnerController() const;

};
