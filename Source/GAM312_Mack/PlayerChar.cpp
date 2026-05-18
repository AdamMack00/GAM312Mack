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
}

// Moves char forward and backwards
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

// Moves char left and right
void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
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
