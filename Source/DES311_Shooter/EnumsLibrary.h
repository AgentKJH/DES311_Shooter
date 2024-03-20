// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM()
enum class EweaponState : uint8
{
	Idle,
	PullingHammer,
	HammerPulled,
	Shooting,
};

class DES311_SHOOTER_API EnumsLibrary
{
public:
	EnumsLibrary();
	~EnumsLibrary();


};
