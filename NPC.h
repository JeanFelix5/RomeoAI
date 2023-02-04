// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PatrolPath.h"
#include "NPC.generated.h"


UCLASS()
class UEPROJ_AUTRE_CPP_API ANPC : public AActiveCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	//handles the crouching broadcast from Juliette
	UFUNCTION(BlueprintCallable)
	void CrouchingBroadcastHandler(bool bIsStarting);

	UFUNCTION(BlueprintCallable)
	void JumpingBroadcastHandler(bool bIsStartingJump);

	UFUNCTION(BlueprintCallable)
	void StartCrouching();

	UFUNCTION(BlueprintCallable)
	void StopCrouching();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Hand Holding", DisplayName = "Duree du tenage de main")
		float f_HandHoldingDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Hand Holding", DisplayName = "Duree avant la vibration")
		float f_StartRumbleTime = 4.5f;

	UFUNCTION()
	APatrolPath* get_patrol_path();

	UFUNCTION()
	void set_patrol_path(APatrolPath* nextPatrolPath);

	UFUNCTION()
		void SetMaxSpeed(float maxSpeed);

	UFUNCTION()
		void ResetMaxSpeed() {
		this->GetCharacterMovement()->MaxWalkSpeed = npc_WalkSpeed;

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float npc_WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float npc_CrouchSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UForceFeedbackEffect* rumble = nullptr;

	UFUNCTION()
		bool getIsWinter() {
		return this->bisWinter;

	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		APatrolPath* patrol_path = nullptr;
};
