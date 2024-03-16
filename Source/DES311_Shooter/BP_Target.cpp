// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_Target.h"
#include "Engine/DamageEvents.h"


// Sets default values
ABP_Target::ABP_Target()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMeshComponent;

	
}

// Called when the game starts or when spawned
void ABP_Target::BeginPlay()
{
	Super::BeginPlay();
	staticMeshComponent->SetSimulatePhysics(true);

}

// Called every frame
void ABP_Target::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABP_Target::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	//Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);

	GEngine->AddOnScreenDebugMessage(20, 5, FColor::Red, "TakeDamage Target"); // print taking damage

	if (damageEvent.IsOfType(FPointDamageEvent::ClassID)) { // check is of point damage type
		GEngine->AddOnScreenDebugMessage(22, 5, FColor::Red, "is of type pointDamage");

		const FPointDamageEvent& pointDamageEvent = *static_cast<const FPointDamageEvent*>(&damageEvent); // cast to convert to point damage type

		//GEngine->AddOnScreenDebugMessage(10, 5, FColor::Green, FString::SanitizeFloat(damageAmount));
		//GEngine->AddOnScreenDebugMessage(11, 5, FColor::Green, pointDamageEvent.ShotDirection.ToString());


		hitForceMultipler = 2000;
		FVector force = pointDamageEvent.ShotDirection * hitForceMultipler;

		FString text;
		text = TEXT("Force: %s"), force.ToString();
		GEngine->AddOnScreenDebugMessage(23, 5, FColor::Yellow, force.ToString());

		staticMeshComponent->AddImpulseAtLocation(force, pointDamageEvent.HitInfo.Location);
	}

	return damageAmount;
}

void ABP_Target::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);


	// Add the static mesh component to the Components list
	if (staticMeshComponent && !staticMeshComponent->IsRegistered())
	{
		staticMeshComponent->RegisterComponent();

	}
}

