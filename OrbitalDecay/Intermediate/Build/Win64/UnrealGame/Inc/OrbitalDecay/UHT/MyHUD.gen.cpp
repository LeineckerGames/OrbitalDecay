// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OrbitalDecay/MyHUD.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMyHUD() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AHUD();
ORBITALDECAY_API UClass* Z_Construct_UClass_AMyHUD();
ORBITALDECAY_API UClass* Z_Construct_UClass_AMyHUD_NoRegister();
ORBITALDECAY_API UClass* Z_Construct_UClass_UMathGenerator_NoRegister();
UPackage* Z_Construct_UPackage__Script_OrbitalDecay();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AMyHUD Function SetQuestion **********************************************
struct Z_Construct_UFunction_AMyHUD_SetQuestion_Statics
{
	struct MyHUD_eventSetQuestion_Parms
	{
		FString Type;
		int32 Level;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "MyHUD.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetQuestion constinit property declarations ***************************
	static const UECodeGen_Private::FStrPropertyParams NewProp_Type;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Level;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetQuestion constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetQuestion Property Definitions **************************************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(MyHUD_eventSetQuestion_Parms, Type), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::NewProp_Level = { "Level", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(MyHUD_eventSetQuestion_Parms, Level), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::NewProp_Type,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::NewProp_Level,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::PropPointers) < 2048);
// ********** End Function SetQuestion Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AMyHUD, nullptr, "SetQuestion", 	Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::MyHUD_eventSetQuestion_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020601, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::Function_MetaDataParams), Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::MyHUD_eventSetQuestion_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AMyHUD_SetQuestion()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMyHUD_SetQuestion_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AMyHUD::execSetQuestion)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_Type);
	P_GET_PROPERTY(FIntProperty,Z_Param_Level);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetQuestion(Z_Param_Type,Z_Param_Level);
	P_NATIVE_END;
}
// ********** End Class AMyHUD Function SetQuestion ************************************************

// ********** Begin Class AMyHUD *******************************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AMyHUD;
UClass* AMyHUD::GetPrivateStaticClass()
{
	using TClass = AMyHUD;
	if (!Z_Registration_Info_UClass_AMyHUD.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("MyHUD"),
			Z_Registration_Info_UClass_AMyHUD.InnerSingleton,
			StaticRegisterNativesAMyHUD,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AMyHUD.InnerSingleton;
}
UClass* Z_Construct_UClass_AMyHUD_NoRegister()
{
	return AMyHUD::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AMyHUD_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Rendering Actor Input Replication" },
		{ "IncludePath", "MyHUD.h" },
		{ "ModuleRelativePath", "MyHUD.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MathEngine_MetaData[] = {
		{ "ModuleRelativePath", "MyHUD.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentLevel_MetaData[] = {
		{ "Category", "Math Settings" },
		{ "ModuleRelativePath", "MyHUD.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_QuestionType_MetaData[] = {
		{ "Category", "Math Settings" },
		{ "ModuleRelativePath", "MyHUD.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FuelRewardAmount_MetaData[] = {
		{ "Category", "Math Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \"a\" = add, \"s\" = sub, \"m\" = mul, \"d\" = div\n" },
#endif
		{ "ModuleRelativePath", "MyHUD.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\"a\" = add, \"s\" = sub, \"m\" = mul, \"d\" = div" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class AMyHUD constinit property declarations ***********************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MathEngine;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CurrentLevel;
	static const UECodeGen_Private::FStrPropertyParams NewProp_QuestionType;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_FuelRewardAmount;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class AMyHUD constinit property declarations *************************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("SetQuestion"), .Pointer = &AMyHUD::execSetQuestion },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AMyHUD_SetQuestion, "SetQuestion" }, // 268844709
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMyHUD>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AMyHUD_Statics

// ********** Begin Class AMyHUD Property Definitions **********************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AMyHUD_Statics::NewProp_MathEngine = { "MathEngine", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMyHUD, MathEngine), Z_Construct_UClass_UMathGenerator_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MathEngine_MetaData), NewProp_MathEngine_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AMyHUD_Statics::NewProp_CurrentLevel = { "CurrentLevel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMyHUD, CurrentLevel), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentLevel_MetaData), NewProp_CurrentLevel_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_AMyHUD_Statics::NewProp_QuestionType = { "QuestionType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMyHUD, QuestionType), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_QuestionType_MetaData), NewProp_QuestionType_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AMyHUD_Statics::NewProp_FuelRewardAmount = { "FuelRewardAmount", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMyHUD, FuelRewardAmount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FuelRewardAmount_MetaData), NewProp_FuelRewardAmount_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AMyHUD_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMyHUD_Statics::NewProp_MathEngine,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMyHUD_Statics::NewProp_CurrentLevel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMyHUD_Statics::NewProp_QuestionType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMyHUD_Statics::NewProp_FuelRewardAmount,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMyHUD_Statics::PropPointers) < 2048);
// ********** End Class AMyHUD Property Definitions ************************************************
UObject* (*const Z_Construct_UClass_AMyHUD_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AHUD,
	(UObject* (*)())Z_Construct_UPackage__Script_OrbitalDecay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMyHUD_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AMyHUD_Statics::ClassParams = {
	&AMyHUD::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AMyHUD_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AMyHUD_Statics::PropPointers),
	0,
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMyHUD_Statics::Class_MetaDataParams), Z_Construct_UClass_AMyHUD_Statics::Class_MetaDataParams)
};
void AMyHUD::StaticRegisterNativesAMyHUD()
{
	UClass* Class = AMyHUD::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_AMyHUD_Statics::Funcs));
}
UClass* Z_Construct_UClass_AMyHUD()
{
	if (!Z_Registration_Info_UClass_AMyHUD.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMyHUD.OuterSingleton, Z_Construct_UClass_AMyHUD_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AMyHUD.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AMyHUD);
AMyHUD::~AMyHUD() {}
// ********** End Class AMyHUD *********************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MyHUD_h__Script_OrbitalDecay_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AMyHUD, AMyHUD::StaticClass, TEXT("AMyHUD"), &Z_Registration_Info_UClass_AMyHUD, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMyHUD), 2466158183U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MyHUD_h__Script_OrbitalDecay_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MyHUD_h__Script_OrbitalDecay_3919656216{
	TEXT("/Script/OrbitalDecay"),
	Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MyHUD_h__Script_OrbitalDecay_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MyHUD_h__Script_OrbitalDecay_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
