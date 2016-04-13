// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "BGHGameMode.h"
#include "HunterConditionHUD.h"



ABGHGameMode::ABGHGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = AHunterConditionHUD::StaticClass();
}
