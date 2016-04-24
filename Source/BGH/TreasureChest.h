// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TreasureChest.generated.h"

UCLASS()
class BGH_API ATreasureChest : public AActor
{
	GENERATED_BODY()

		
	
public:	
	// Sets default values for this actor's properties
	ATreasureChest();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Sprite for the body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Treasure", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* ActiveSprite;

	//Sprite for the body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Treasure", meta = (AllowPrivateAccess = "true"))
	class UPaperSprite* ClosedTreasureSprite;

	//Sprite for the body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Treasure", meta = (AllowPrivateAccess = "true"))
	class UPaperSprite* OpenedTreasureSprite;

	//Sprite for the body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Treasure", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* OpenTreasureFB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Treasure", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetupPlayerInputComponent(class UInputComponent* InputComponent);

	void Open();

private:

	bool bIsOpen;

	bool bIsPlayerOverlaping;
	
};
