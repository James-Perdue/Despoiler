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
enum class EAttackState : uint8
{
	Idle UMETA(DisplayName = "Starting"),
	Ready UMETA(DisplayName = "Ready"),
	Cooldown UMETA(DisplayName = "CoolDown"),
};