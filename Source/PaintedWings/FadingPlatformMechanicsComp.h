// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FadingPlatformMechanicsComp.generated.h"

class UStaticMeshComponent;
class UMaterial;
class UMaterialInstanceDynamic;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAINTEDWINGS_API UFadingPlatformMechanicsComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFadingPlatformMechanicsComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	int OpacityChange;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* platformMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* MyMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* DynMaterial;
};
