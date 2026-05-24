// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Iniital setup of camera component
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// attatching camera to character mesh and head bone 
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	// share rotation with controller
	PlayerCamComp->bUsePawnControlRotation = true;

	// Sets array size for 3 building options.
	BuildingArray.SetNum(3);

	// Sets array size for 3 resources.
	ResourcesArray.SetNum(3);

	// Adds resource names.
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	// Creates timer handle for stat updates.
	FTimerHandle StatsTimerHandle;

	// Runs DecreaseStats every 2 seconds.
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Checks if the player is currently placing a building part.
	if (isBuilding)
	{
		// Makes sure there is a spawned building part before trying to move it.
		if (spawnedPart)
		{
			// Gets the current location of the player's camera.
			FVector StartLocation = PlayerCamComp->GetComponentLocation();

			// Gets the forward direction from the camera and extends it 400 units outward.
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;

			// Calculates the final location in front of the player.
			FVector EndLocation = StartLocation + Direction;

			// Moves the building part so it stays in front of the player while being placed.
			spawnedPart->SetActorLocation(EndLocation);
		}
	}
}

void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	// Calls the parent class version 
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement input
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);

	// Mouse look input
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);

	// Jump input
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

	// interaction input
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerChar::FindObject);

	// Rotate Input
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);

}

// Moves char forward and backwards
void APlayerChar::MoveForward(float axisValue)
{
	// Gets the forward direction based on the player's controller rotation.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	// Applies movement input forward or backward depending on the axis value.
	AddMovementInput(Direction, axisValue);
}

// Moves char left and right
void APlayerChar::MoveRight(float axisValue)
{
	// Gets the right direction based on the player's controller rotation.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

	// Applies movement input left or right depending on the axis value.
	AddMovementInput(Direction, axisValue);
}


// Starts the jump when jump button is pressed
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

// stops jump when jump button is released
void APlayerChar::StopJump()
{
	bPressedJump = false;
}

void APlayerChar::FindObject()
{
	// Stores line trace hit data.
	FHitResult HitResult;

	// Sets trace start from player camera.
	FVector StartLocation = PlayerCamComp->GetComponentLocation();

	// Sets trace direction and distance.
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;

	// Sets trace end point.
	FVector EndLocation = StartLocation + Direction;

	// Sets trace options.
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	// If the player is not building, interact will look for resources.
	if (!isBuilding) {
		// Runs line trace.
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			// Checks if hit actor is a resource.
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			// Requires enough stamina to collect.
			if (Stamina > 5.0f)
			{
				if (HitResource)
				{
					// Gets resource info.
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					// Removes amount from resource.
					HitResource->totalResource = HitResource->totalResource - resourceValue;

					// Collects resource if not depleted.
					if (HitResource->totalResource > resourceValue)
					{
						GiveResource(resourceValue, hitName);

						// Prints collected message.
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						// Spawns hit decal.
						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						// Drains stamina.
						SetStamina(-5.0f);
					}
					else
					{
						// Removes depleted resource.
						HitResource->Destroy();

						// Prints depleted message.
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
					}
				}
			}
		}
	}
	else
	{
		// Turns off building mode when the player confirms placement.
		isBuilding = false;

		//objectsBuilt = objectsBuilt + 1.0f;

		//objWidget->UpdatebuildObj(objectsBuilt);
	}
}

// Changes player health.
void APlayerChar::SetHealth(float amount)
{
	// Keeps health under 100.
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}

// Changes player hunger.
void APlayerChar::SetHunger(float amount)
{
	// Keeps hunger under 100.
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}

// Changes player stamina.
void APlayerChar::SetStamina(float amount)
{
	// Keeps stamina at or under 100.
	if (Stamina + amount <= 100)
	{
		Stamina = Stamina + amount;
	}
}

// Updates stats over time.
void APlayerChar::DecreaseStats()
{
	// Lowers hunger if above 0.
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}

	// Restores stamina over time.
	SetStamina(10.0f);

	// Damages player if hunger is empty.
	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}

// Adds resources to the player.
void APlayerChar::GiveResource(float amount, FString resourceType)
{
	// Adds wood.
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	// Adds stone.
	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	// Adds berries.
	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	// Checks if the player has enough wood for the selected building object.
	if (woodAmount <= ResourcesArray[0])
	{
		// Checks if the player has enough stone for the selected building object.
		if (stoneAmount <= ResourcesArray[1])
		{
			// Subtracts the required wood from the player's resource inventory.
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;

			// Subtracts the required stone from the player's resource inventory.
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			// Adds one wall piece to the player's building inventory.
			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}

			// Adds one floor piece to the player's building inventory.
			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}

			// Adds one ceiling piece to the player's building inventory.
			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	}
}

void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	// Only allows a new building part to spawn if the player is not already placing one.
	if (!isBuilding)
	{
		// Checks if the player has at least one of the selected building parts available.
		if (BuildingArray[buildingID] >= 1)
		{
			// Turns on building mode so the spawned part can follow the player's camera.
			isBuilding = true;

			// Stores spawn settings for the building part.
			FActorSpawnParameters SpawnParams;

			// Gets the current location of the player's camera.
			FVector StartLocation = PlayerCamComp->GetComponentLocation();

			// Gets the forward direction from the camera and extends it 400 units outward.
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;

			// Calculates the spawn location in front of the player.
			FVector EndLocation = StartLocation + Direction;

			// Sets the starting rotation for the spawned building part.
			FRotator myRot(0, 0, 0);

			// Removes one of the selected building parts from the player's building inventory.
			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			// Spawns the selected building part in front of the player.
			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			// Returns true because the building part was successfully spawned.
			isSuccess = true;
		}
		else
		{
			// Returns false if the player does not have the selected building part available.
			isSuccess = false;
		}
	}
	else
	{
		// Returns false if the player is already placing a building part.
		isSuccess = false;
	}
}

void APlayerChar::RotateBuilding()
{
	// Only rotates the building part while the player is actively placing one.
	if (isBuilding)
	{
		// Rotates the spawned building part 90 degrees around the yaw axis.
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}