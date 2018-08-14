// Fill out your copyright notice in the Description page of Project Settings.

#include "BirdPlayer.h"


// Sets default values
ABirdPlayer::ABirdPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABirdPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABirdPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABirdPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

