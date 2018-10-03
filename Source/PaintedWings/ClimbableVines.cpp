// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbableVines.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "Player/BirdPlayer.h"
#include "Player/BirdController.h"

// Sets default values
AClimbableVines::AClimbableVines()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VinesTrigger = CreateDefaultSubobject<UBoxComponent>("Box");
	VinesTrigger->OnComponentBeginOverlap.AddDynamic(this, &AClimbableVines::OnOverlapBegin);
	VinesTrigger->OnComponentEndOverlap.AddDynamic(this, &AClimbableVines::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AClimbableVines::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClimbableVines::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClimbableVines::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		BirdRef -> bClimbingVines = true;
		BirdRef->JumpCurrentCount = 1;
		BirdRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		BirdRef->GetCharacterMovement()->bOrientRotationToMovement = false;
		BirdRef->DirectionToVine = -GetActorForwardVector();// GetActorLocation() - BirdRef->GetActorLocation();
		//BirdRef->DirectionToVine.Normalize();
		FRotator NewRotation = BirdRef->GetActorRotation();
		NewRotation.Yaw = GetActorRightVector().Rotation().Yaw - 180;
		BirdRef->SetActorRotation(NewRotation);
		BirdRef -> HasDoubleJumped = false;
		BirdRef->bSimGravityDisabled = true;
		UE_LOG(LogTemp, Warning, TEXT("VinesEntering: %s"), BirdRef->HasDoubleJumped ? TEXT("true") : TEXT("false"));
	}
}

void AClimbableVines::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		//BirdRef -> bClimbingVines = false;
		////BirdRef -> HasDoubleJumped = false;
		//BirdRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		//BirdRef->bSimGravityDisabled = false;
		//UE_LOG(LogTemp, Warning, TEXT("VinesLeaving"));
	}
}

