// Fill out your copyright notice in the Description page of Project Settings.


#include "GAM312GameMode.h"
#include "PlayerChar.h"

AGAM312GameMode::AGAM312GameMode()
{
	DefaultPawnClass = APlayerChar::StaticClass();
}
