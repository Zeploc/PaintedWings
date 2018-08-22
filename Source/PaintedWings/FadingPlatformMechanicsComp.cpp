// Fill out your copyright notice in the Description page of Project Settings.

#include "FadingPlatformMechanicsComp.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine.h"
// Sets default values for this component's properties
UFadingPlatformMechanicsComp::UFadingPlatformMechanicsComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFadingPlatformMechanicsComp::BeginPlay()
{
	Super::BeginPlay();
	//platformMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	//static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/DatasmithContent/Materials/FBXImporter/DeltaGenMasterAbsorb.DeltaGenMasterAbsorb'"));
	//if (Material.Object != NULL)
	//{
	//	MyMaterial = (UMaterial*)Material.Object;
	//}
	//OpacityChange = 1000;
	//
	//DynMaterial = UMaterialInstanceDynamic::Create(MyMaterial, this);
	//platformMesh->SetMaterial(0, DynMaterial);
	// ...
	
}


// Called every frame
void UFadingPlatformMechanicsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (OpacityChange > 0)
	{
		OpacityChange--;
	}
	if (OpacityChange == 0)
	{
		OpacityChange = 1000;
	}
	// ...
}

