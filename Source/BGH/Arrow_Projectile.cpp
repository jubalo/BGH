// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "Arrow_Projectile.h"


// Sets default values
AArrow_Projectile::AArrow_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArrow_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow_Projectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

