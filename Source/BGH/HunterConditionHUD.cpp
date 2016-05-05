// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "HunterConditionHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"


AHunterConditionHUD::AHunterConditionHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CharacterCondTexObj(TEXT("/Game/Player_Content/character_HUD"));
	CharacterCondTex = CharacterCondTexObj.Object;
}

void AHunterConditionHUD::DrawHUD()
{
	Super::DrawHUD();

	//
	const FVector2D Position(10.0f, 10.0f);
	

	// draw HUD
	FCanvasTileItem TileItem(Position, CharacterCondTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	DrawRect(FLinearColor(0.0f, 0.8f, 0.0f, 1.0f), 70.0f, 70.0f, CharacterCondTex->GetSizeX() - 60.0f , 10.0f);
	DrawRect(FLinearColor(0.8f, 0.8f, 0.0f, 1.0f), 70.0f, 90.0f, CharacterCondTex->GetSizeX() - 60.0f, 10.0f);
	Canvas->DrawItem(TileItem);
}