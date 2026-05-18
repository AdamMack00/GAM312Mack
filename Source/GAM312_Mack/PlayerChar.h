// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
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
	UFUNCTION()
	void MoveForward(float axisValue);

	UFUNCTION()
	void MoveRight(float axisValue);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

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

};
