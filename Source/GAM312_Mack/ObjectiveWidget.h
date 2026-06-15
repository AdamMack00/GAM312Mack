// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_MACK_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    // Blueprint event used to update the objective UI with the amount of materials collected.
    UFUNCTION(BlueprintImplementableEvent)
    void UpdatematOBJ(float matsCollected);

    // Blueprint event used to update the objective UI with the amount of objects built.
    UFUNCTION(BlueprintImplementableEvent)
    void UpdatebuildObj(float objectsBuilt);
	
};
