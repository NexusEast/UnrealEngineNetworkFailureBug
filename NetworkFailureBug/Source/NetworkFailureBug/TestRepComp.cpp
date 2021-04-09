// Fill out your copyright notice in the Description page of Project Settings.
 
#include "TestRepComp.h"

#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogNTC)
// Sets default values for this component's properties
UTestRepComp::UTestRepComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval = 0.5f;  

	UE_LOG(LogNTC, Log, TEXT(" "));
}

// Called every frame
void UTestRepComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwner()->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		if (CurrentState == 0)
		{
			RootProp.Empty();
			CurrentState++;
			UE_LOG(LogNTC, Log, TEXT("flipflop==0"));
			DumpRep();
		}
		else if (CurrentState == 1)
		{ 
			UE_LOG(LogNTC, Log, TEXT("flipflop==1")); 
			FNTCStruct s;
			CurrentState++; 
			RootProp.Add(s);
			Counter++;
			DumpRep();
		} 
	} 
}

void UTestRepComp::OnRep_RootProp()
{
	UE_LOG(LogNTC, Log, TEXT("OnRep_RootProp"));
	DumpRep();
}

void UTestRepComp::CountUp_Implementation()
{
	CurrentState = 0;
}

bool UTestRepComp::CountUp_Validate()
{
	return true;
}

void UTestRepComp::AddOne_Implementation()
{ 
	RootProp.Empty();
	FNTCStruct s;
	s.SubArray.Add(Counter);
	RootProp.Add(s);
	Counter++;
}

bool UTestRepComp::AddOne_Validate()
{
	return true;
}

void UTestRepComp::EmptySubArray_Implementation()
{
	if (!!RootProp.Num())
	{
		RootProp[0].SubArray.Empty();
	}
}

bool UTestRepComp::EmptySubArray_Validate()
{
	return true;
}

void UTestRepComp::DumpRep()
{
	UE_LOG(LogNTC, Log, TEXT("\tRootProp:[%p],Num:%d"), &RootProp, RootProp.Num());
	for (const FNTCStruct& SubStruct : RootProp)
	{
		UE_LOG(LogNTC, Log, TEXT("\t\tNumbers:[%p],Num:%d"), &SubStruct, SubStruct.SubArray.Num());
		for (int32 i : SubStruct.SubArray)
		{
			UE_LOG(LogNTC, Log, TEXT("\t\t\tn:%d"), i);
		}
	}
}

void UTestRepComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(UTestRepComp, RootProp);  
}

