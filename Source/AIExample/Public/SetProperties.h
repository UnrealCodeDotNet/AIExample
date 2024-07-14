
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

// utilities for setting properties

inline bool SetBlackboardKeySelectorProperty( UObject * Target, const FName& VarName, const FBlackboardKeySelector& NewValue )
{
	if (Target)
	{
		if (const FStructProperty* Prop = FindFieldChecked<FStructProperty>(Target->GetClass(), VarName ))
		{
			Prop->SetValue_InContainer( Target, &NewValue ); 
			return true;
		}
	}
	return false;
}	

inline bool SetBoolProperty( UObject * Target, const FName& VarName, const bool NewValue )
{
	if (Target)
	{
		if (const FBoolProperty* Prop = FindFieldChecked<FBoolProperty>(Target->GetClass(), VarName ))
		{
			Prop->SetPropertyValue_InContainer( Target, NewValue ); 
			return true;
		}
	}
	return false;
}	
