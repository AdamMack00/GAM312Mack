// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_MACK_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Blueprint event used to update the player's health, hunger, and stamina bars
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateBars(float Health1, float Hunger1, float Stamina1);

};
