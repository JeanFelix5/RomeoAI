// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "blackboard_keys.h"
#include "Components/BoxComponent.h"
#include "NPC.h"
#include "NPC_AIController.h"
#include "UEProj_autre_cppCharacter.h"
#include "WaitAI.generated.h"

UCLASS()
class UEPROJ_AUTRE_CPP_API AWaitAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaitAI();

	UFUNCTION()
	void onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void onEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
		float f_Time = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* CollisionBox = nullptr;

private:	
	
	UPROPERTY()
		ANPC* npc;

	UPROPERTY()
		ANPC_AIController* controller;

	UPROPERTY()
		AUEProj_autre_cppCharacter* juliette;
	
};
