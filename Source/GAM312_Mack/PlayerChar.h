// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerChar.generated.h"


UCLASS()
class GAM312_MACK_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Player movement and interaction functions 

	// Moves the player forward and backward based on input.
	UFUNCTION()
	void MoveForward(float axisValue);

	// Moves the player left and right based on input.
	UFUNCTION()
	void MoveRight(float axisValue);

	// Starts the player jump.
	UFUNCTION()
	void StartJump();

	// Stops the player jump.
	UFUNCTION()
	void StopJump();

	// Checks what object the player is looking at and handles interaction.
	UFUNCTION()
	void FindObject();

	// camera compnent attatched to the player
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;


	// Player health value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;

	// Player hunger value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;

	// Player stamina value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;

	// Wood resource count.
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;

	// Stone resource count.
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;

	// Berry resource count.
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;

	// Stores resource amounts.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	// Stores resource names.
	UPROPERTY(EditAnywhere, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	// Decal used when hitting resources.
	UPROPERTY(EditAnywhere, Category = "HitMarker")
	UMaterialInterface* hitDecal;

	// Changes player health.
	UFUNCTION(BlueprintCallable)
	void SetHealth(float amount);

	// Changes player hunger.
	UFUNCTION(BlueprintCallable)
	void SetHunger(float amount);

	// Changes player stamina.
	UFUNCTION(BlueprintCallable)
	void SetStamina(float amount);

	// Decreases player stats over time.
	UFUNCTION()
	void DecreaseStats();

	// Adds resources to the player.
	UFUNCTION()
	void GiveResource(float amount, FString resourceType);

	// Stores how many building pieces the player has available to place.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
	TArray<int> BuildingArray;

	// Tracks whether the player is currently placing a building part.
	UPROPERTY()
	bool isBuilding;

	// Stores the building part Blueprint class that will be spawned.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABuildingPart> BuildPartClass;

	// Stores a reference to the building part currently being placed.
	UPROPERTY()
	ABuildingPart* spawnedPart;

	// Updates the player's resources when crafting a building object.
	UFUNCTION(BlueprintCallable)
	void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	// Spawns the selected building part and reports whether it was successful.
	UFUNCTION(BlueprintCallable)
	void SpawnBuilding(int buildingID, bool& isSuccess);

	// Rotates the currently spawned building part while it is being placed.
	UFUNCTION()
	void RotateBuilding();
};