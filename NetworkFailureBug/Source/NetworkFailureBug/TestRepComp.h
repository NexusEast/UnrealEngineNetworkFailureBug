// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestRepComp.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogNTC, Log, All);
USTRUCT(BlueprintType)
struct FNTCStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		TArray<	int32 > SubArray;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class  UTestRepComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestRepComp();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(ReplicatedUsing = OnRep_RootProp)
		TArray<FNTCStruct> RootProp; 

	UFUNCTION()
		void OnRep_RootProp();

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
		void CountUp();

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
		void AddOne();

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
		void EmptySubArray();

private:

	void DumpRep(); 

private:

	int32 Counter = 0;   
	int32 CurrentState = -1;
	
};
