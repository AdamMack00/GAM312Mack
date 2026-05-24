// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "BuildingPart.generated.h"

UCLASS()
class GAM312_MACK_API ABuildingPart : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mesh component used to display the visual model for the building part.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	// Arrow component used as the root and pivot point for placing/rotating the building part.
	UPROPERTY(EditAnywhere)
	UArrowComponent* PivotArrow;

};