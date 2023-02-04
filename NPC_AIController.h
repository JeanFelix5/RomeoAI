// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionTypes.h"
#include "NPC_AIController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandHoldStartDelegate, bool, bIsStarting);

UCLASS()
class UEPROJ_AUTRE_CPP_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANPC_AIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());

	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;
	class UBlackboardComponent* get_blackboard() const;

	void SetNPCPushStatus(bool status);

	UPROPERTY()
		class ANPC* npc;

	UPROPERTY()
		ACharacter* JulietteRef;
	
	UFUNCTION()
		FVector GetNPCLocation();

	UFUNCTION()
		void OnBeginHoldingHands();

	UFUNCTION()
		void InitializeJulietteRef() {JulietteRef = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);}

	UFUNCTION()
		void StopHoldingHands();

	UFUNCTION()
		void OnBeginWaiting(float SecondsToWait);

	UFUNCTION()
		void OnEndWaiting(float SecondsToWait);

	UFUNCTION(BlueprintCallable)
		void SimpleRumble();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float f_WaitingTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Air Movement")
		float AirControl = 0.2f;

	/** Event Dispatcher for holding hand for flowers*/
	UPROPERTY(BlueprintAssignable)
		FHandHoldStartDelegate OnHandHoldHandler;

private:
	bool b_IsRumbling = false;
	FName HHRumble = "Rumble";

	//if Romeo is burnt out from holding hands
	bool b_BurntOutHH = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* behavior_tree_component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* behavior_tree;

	class UBlackboardComponent* blackboard;

	class UAISenseConfig_Sight* sight_config;

	UFUNCTION()
		void on_updated(TArray<AActor*> const& updated_actors);

	UFUNCTION()
	void setup_perception_system();

	UFUNCTION()
		void on_target_detected(AActor* actor, FAIStimulus const stimulus);

	UFUNCTION()
		void OnMovementModeChanged(ACharacter* MovedCharacter, EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0);

	UFUNCTION()
		void OnLanded(const FHitResult& Hit);

	UPROPERTY()
		float f_TimeElapsed = 0.0f;

	UPROPERTY()
		float f_TimeElapsedHH = 0.0f;

	UPROPERTY()
		float f_timerRefreshDuration = 0.1f;

	//Function that increments the hand holding timer 
	UFUNCTION()
		void HandHoldingTimerRepeatable();

	//Function that decrements the hand holding timer 
	UFUNCTION()
		void HandHoldingCooldownTimerRepeatable();


	UPROPERTY()
		FTimerHandle localTimerHandle = FTimerHandle();
	
	UFUNCTION()
		void TimerRepeatableWaiting();

	UPROPERTY()
		FTimerHandle localTimerHandleWaiting = FTimerHandle();
};
