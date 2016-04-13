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

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Position(10.0f, 10.0f);
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D CrosshairDrawPosition((Center.X - (CharacterCondTex->GetSurfaceWidth() * 0.5)),
		(Center.Y - (CharacterCondTex->GetSurfaceHeight() * 0.5f)));

	// draw the crosshair
	FCanvasTileItem TileItem(Position, CharacterCondTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	DrawRect(FLinearColor(0.0f, 0.8f, 0.0f, 1.0f), 70.0f, 70.0f, CharacterCondTex->GetSizeX() - 60.0f , 10.0f);
	DrawRect(FLinearColor(0.8f, 0.8f, 0.0f, 1.0f), 70.0f, 90.0f, CharacterCondTex->GetSizeX() - 60.0f, 10.0f);
	Canvas->DrawItem(TileItem);
}