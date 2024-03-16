// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_Target.generated.h"

UCLASS()
class DES311_SHOOTER_API ABP_Target : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SMesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* staticMeshComponent;
	
public:	
	// Sets default values for this actor's properties
	ABP_Target();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = "Hit")
	float hitForceMultipler = 100;
};
