// Fill out your copyright notice in the Description page of Project Settings.

#include "myPlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "playerCheckpointMechanics.h"
// Sets default values
AmyPlayerCharacter::AmyPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AmyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AmyPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Respawn", IE_Pressed,this, &AmyPlayerCharacter::MoveToCheckpoint);
}


// Called every frame
void AmyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AmyPlayerCharacter::MoveToCheckpoint()
{
	if (GetOwner())
	{
		if (GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>())
		{
			GetOwner()->FindComponentByClass<UplayerCheckpointMechanics>()->MoveToCurrentCheckpoint();
		}
	}
}

