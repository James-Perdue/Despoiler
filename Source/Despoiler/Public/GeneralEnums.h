// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EActionStatus : uint8
{
	Completed UMETA(DisplayName = "Completed"),
	Failed UMETA(DisplayName = "Failed"),
	Running UMETA(DisplayName = "Running"),
	Idle UMETA(DisplayName = "Idle")
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Attacker UMETA(DisplayName = "Attacker"),
	Defender UMETA(DisplayName = "Defender")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle UMETA(DisplayName = "Starting"),
	Ready UMETA(DisplayName = "Ready"),
	Cooldown UMETA(DisplayName = "CoolDown"),
};

UENUM(BlueprintType)
enum class EAgentState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Attacking UMETA(DisplayName = "Attacking"),
	Defending UMETA(DisplayName = "Defending"),
	Capturing UMETA(DisplayName = "Capturing"),
	Stunned UMETA(DisplayName = "Stunned")
};

UENUM(BlueprintType)
enum class ESquadState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Attack UMETA(DisplayName = "Attack"),
	HoldPosition UMETA(DisplayName = "HoldPosition"),
	Capture UMETA(DisplayName = "Capture"),
	Free UMETA(DisplayName = "Free"),
	Retreat UMETA(DisplayName = "Retreat")
};