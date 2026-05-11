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

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
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

// Interaction function placeholder
void APlayerChar::FindObject()
{
}

