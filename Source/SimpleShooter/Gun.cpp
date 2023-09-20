// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	//RootComponent = RootComp;
	SetRootComponent(RootComp);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::GunTrace(FHitResult& TargetPoint, FVector& ShotDirection)
{
	/*APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;*/

	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) 
		return false;
	FVector StartVec; //Start point
	FRotator StartRot; // Start point rotation

	OwnerController->GetPlayerViewPoint(StartVec, StartRot); //Uses out parameters
	FVector EndPoint = StartVec + StartRot.Vector() * MaxRange;
	ShotDirection = -StartRot.Vector();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	//Params.AddIgnoredActor(OwnerPawn); //Or Params.AddIgnoredActor(GetOwner()); 

	return GetWorld()->LineTraceSingleByChannel(TargetPoint, StartVec, EndPoint, ECC_GameTraceChannel1, Params); //FCollisionQueryParams could be ignored here.
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	
	return OwnerPawn->GetController();
}

void AGun::PullTrigger()
{
	//UE_LOG(LogTemp, Warning, TEXT("You've been shot!"))

	UGameplayStatics::SpawnEmitterAttached(ShootParticles, GunMesh, TEXT("MuzzleFlashSocket"));//, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	UGameplayStatics::SpawnSoundAttached(ShootSound, GunMesh, TEXT("MuzzleFlashSocket"));
	

	//APawn* OwnerPawn = Cast<APawn>(GetOwner());
	//if (OwnerPawn == nullptr) return;
	//AController* OwnerController = OwnerPawn->GetController();
	//if (OwnerController == nullptr) return;
	//FVector StartVec; //Start point
	//FRotator StartRot; // Start point rotation
	//FHitResult TargetPoint;

	//OwnerController->GetPlayerViewPoint(StartVec, StartRot); //Uses out parameters
	//FVector EndPoint = StartVec + StartRot.Vector() * MaxRange;
	//FVector ShotDirection = -StartRot.Vector();

	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(this);
	//Params.AddIgnoredActor(OwnerPawn); //Or Params.AddIgnoredActor(GetOwner()); 

	//bool bSuccess = GetWorld()->LineTraceSingleByChannel(TargetPoint, StartVec, EndPoint, ECC_GameTraceChannel1, Params); //FCollisionQueryParams could be ignored here.
	FHitResult TargetPoint;
	FVector ShotDirection;
	
	bool bSuccess = GunTrace(TargetPoint, ShotDirection);
	if (bSuccess)
	{
		DrawDebugPoint(GetWorld(), TargetPoint.ImpactPoint, 20, FColor::Red, true);

		if (Cast<APawn>(TargetPoint.GetActor()) == nullptr)
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, TargetPoint.ImpactPoint);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, TargetPoint.ImpactPoint);

		if (HitParticles) UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, TargetPoint.ImpactPoint, ShotDirection.Rotation());

		if (TargetPoint.GetActor() != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, TargetPoint, ShotDirection, nullptr);
			TargetPoint.GetActor()->TakeDamage(Damage, DamageEvent, GetOwnerController()/*OwnerController*/, this);
		} 
		
	} 

	//DrawDebugCamera(GetWorld(), StartVec, StartRot, 90, 1, FColor::Red, true);
}

