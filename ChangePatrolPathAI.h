// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.h"
#include "Components/BoxComponent.h"
#include "NPC_AIController.h"
#include "ChangePatrolPathAI.generated.h"

UCLASS()
class UEPROJ_AUTRE_CPP_API AChangePatrolPathAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChangePatrolPathAI();

	UFUNCTION()
		void onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		UBoxComponent* CollisionBox = nullptr;

private:

	UPROPERTY()
		ANPC* npc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		APatrolPath* Next_patrol_path = nullptr;

	UPROPERTY()
		ANPC_AIController* controller;
};
