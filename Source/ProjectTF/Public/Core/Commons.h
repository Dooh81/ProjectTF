// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTTF_API Commons
{
public:
	Commons();
	~Commons();
};

template<typename TEnum>
static FORCEINLINE TEnum ConvertIntToEnum(const int32 Value)
{
	return static_cast<TEnum>(Value);
}

template <typename Enumeration>
static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue)).Mid(1);
}

template <typename Enumeration>
static FORCEINLINE FString GetEnumerationToFullString(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue));
}