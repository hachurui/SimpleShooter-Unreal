// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"

#include "Engine/DamageEvents.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

		//Creates scene component and makes it the root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
		//Creates gun mesh and attaches to root
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(Root);
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

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket")); //Spawn muzzle flash particles
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, TEXT("MuzzleFlashSocket"));

	AController* OwnerController = GetOwnerController();
	FHitResult HitResult; //out parameter for GunTrace()
	FVector ShotDirection; //out parameter for GunTrace()
	bool bSuccess = GunTrace(HitResult, ShotDirection); //return a bool that says whether or not we have a successful line trace as well as the two outparameters
	if(bSuccess) //if successful linetrace
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location, ShotDirection.Rotation()); //particles for bullet hitting surfaces
		UGameplayStatics::SpawnSoundAtLocation(this, HitSound, HitResult.Location, ShotDirection.Rotation(), .25);
		FPointDamageEvent DamageEvent(DamageAmount, HitResult, ShotDirection, UDamageType::StaticClass()); //damage event
			//gets the hit actor if there is one and applies damage to them
		AActor* HitActor = HitResult.GetActor();
		if(!HitActor) return;
		HitActor->TakeDamage(DamageAmount, DamageEvent, OwnerController, this);
	}
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection) //performs linetrace from gun's owner's perspective to their aimed at target. Linetrace ignores the gun's owner (can't shoot yourself)
{
	AController* OwnerController = GetOwnerController();
	FVector Location; //out parameter for GetPlayerViewPoint
	FRotator Rotation; //out parameter for GetPlayerViewPoint
	OwnerController->GetPlayerViewPoint(Location, Rotation);
		//calculates the direction the bullet game from so hit particle effect sprays the correct direction and also is needed for damageevents(dont know why)
	ShotDirection = -Rotation.Vector();
	FVector End = Location + (Rotation.Vector() * MaxRange);
		//Declares Collision paramaters for line trace and adds the parameter to ignore the actor that owns this firing gun.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const //its gets the owner of the gun's controller. Need it anymore explained? I'm just messin, but it is that simple
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner()); //Gets called thrice every trigger pull, but the code is readible because of it.. hmm..
	if(!OwnerPawn) return nullptr;
	AController* OwnerController = OwnerPawn->GetController();
	return OwnerController;
}
