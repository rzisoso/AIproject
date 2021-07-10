// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface_AIHelper.h"
#include <Engine/UserDefinedEnum.h>

// Add default functionality here for any IInterface_AIHelper functions that are not pure virtual.

int8 IInterface_AIHelper::GetTeamID()
{
    return 0;
}

bool IInterface_AIHelper::GetisDead()
{
    return false;
}

void IInterface_AIHelper::NotifyStateChanged(FName StateName)
{
}

APatrolPointManager* IInterface_AIHelper::GetPatrol()
{
    return nullptr;
}

void IInterface_AIHelper::SetAnimMode(bool IsWarning)
{

}

float IInterface_AIHelper::GetAttackRange()
{
    return 0.0f;
}
