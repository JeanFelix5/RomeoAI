// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ActiveCharacter.h"
#include "NPC_AIController.h"
#include "PatrolPath.generated.h"


UCLASS()
class UEPROJ_AUTRE_CPP_API APatrolPath : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	APatrolPath();

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionSphere;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (makeEditWidget = "true", AllowPrivateAccess = "true"))
		TArray<FVector> patrol_points;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		bool bLoopingPath = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	int CurrentIndex = 0;

	UPROPERTY()
		AActiveCharacter* npc;

	UPROPERTY()
		ANPC_AIController* controller;


public:
	
	UPROPERTY()
	int nb_points;
	
	UFUNCTION()
	FVector get_patrol_point(int const index) const;

	UFUNCTION()
	int num() const;

	UFUNCTION()
	bool getIsLoopingPath();

	UFUNCTION()
	void onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void ResetCollisionSphereLocation();

	UFUNCTION(BlueprintCallable)
		void ResetWaypointIndex();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
