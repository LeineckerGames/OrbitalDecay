// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OrbitalDecay/MathGenerator.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMathGenerator() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
ORBITALDECAY_API UClass* Z_Construct_UClass_UMathGenerator();
ORBITALDECAY_API UClass* Z_Construct_UClass_UMathGenerator_NoRegister();
ORBITALDECAY_API UScriptStruct* Z_Construct_UScriptStruct_FMathQuestion();
UPackage* Z_Construct_UPackage__Script_OrbitalDecay();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMathQuestion *****************************************************
struct Z_Construct_UScriptStruct_FMathQuestion_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMathQuestion); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMathQuestion); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "ModuleRelativePath", "MathGenerator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_QuestionText_MetaData[] = {
		{ "Category", "MathQuestion" },
		{ "ModuleRelativePath", "MathGenerator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CorrectAnswer_MetaData[] = {
		{ "Category", "MathQuestion" },
		{ "ModuleRelativePath", "MathGenerator.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMathQuestion constinit property declarations *********************
	static const UECodeGen_Private::FStrPropertyParams NewProp_QuestionText;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CorrectAnswer;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMathQuestion constinit property declarations ***********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMathQuestion>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMathQuestion_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMathQuestion;
class UScriptStruct* FMathQuestion::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMathQuestion.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMathQuestion.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMathQuestion, (UObject*)Z_Construct_UPackage__Script_OrbitalDecay(), TEXT("MathQuestion"));
	}
	return Z_Registration_Info_UScriptStruct_FMathQuestion.OuterSingleton;
	}

// ********** Begin ScriptStruct FMathQuestion Property Definitions ********************************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FMathQuestion_Statics::NewProp_QuestionText = { "QuestionText", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMathQuestion, QuestionText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_QuestionText_MetaData), NewProp_QuestionText_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FMathQuestion_Statics::NewProp_CorrectAnswer = { "CorrectAnswer", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMathQuestion, CorrectAnswer), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CorrectAnswer_MetaData), NewProp_CorrectAnswer_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMathQuestion_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMathQuestion_Statics::NewProp_QuestionText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMathQuestion_Statics::NewProp_CorrectAnswer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMathQuestion_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMathQuestion Property Definitions **********************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMathQuestion_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_OrbitalDecay,
	nullptr,
	&NewStructOps,
	"MathQuestion",
	Z_Construct_UScriptStruct_FMathQuestion_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMathQuestion_Statics::PropPointers),
	sizeof(FMathQuestion),
	alignof(FMathQuestion),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMathQuestion_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMathQuestion_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMathQuestion()
{
	if (!Z_Registration_Info_UScriptStruct_FMathQuestion.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMathQuestion.InnerSingleton, Z_Construct_UScriptStruct_FMathQuestion_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMathQuestion.InnerSingleton);
}
// ********** End ScriptStruct FMathQuestion *******************************************************

// ********** Begin Class UMathGenerator ***********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UMathGenerator;
UClass* UMathGenerator::GetPrivateStaticClass()
{
	using TClass = UMathGenerator;
	if (!Z_Registration_Info_UClass_UMathGenerator.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("MathGenerator"),
			Z_Registration_Info_UClass_UMathGenerator.InnerSingleton,
			StaticRegisterNativesUMathGenerator,
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
	return Z_Registration_Info_UClass_UMathGenerator.InnerSingleton;
}
UClass* Z_Construct_UClass_UMathGenerator_NoRegister()
{
	return UMathGenerator::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UMathGenerator_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "MathGenerator.h" },
		{ "ModuleRelativePath", "MathGenerator.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UMathGenerator constinit property declarations ***************************
// ********** End Class UMathGenerator constinit property declarations *****************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMathGenerator>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UMathGenerator_Statics
UObject* (*const Z_Construct_UClass_UMathGenerator_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_OrbitalDecay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMathGenerator_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMathGenerator_Statics::ClassParams = {
	&UMathGenerator::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMathGenerator_Statics::Class_MetaDataParams), Z_Construct_UClass_UMathGenerator_Statics::Class_MetaDataParams)
};
void UMathGenerator::StaticRegisterNativesUMathGenerator()
{
}
UClass* Z_Construct_UClass_UMathGenerator()
{
	if (!Z_Registration_Info_UClass_UMathGenerator.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMathGenerator.OuterSingleton, Z_Construct_UClass_UMathGenerator_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMathGenerator.OuterSingleton;
}
UMathGenerator::UMathGenerator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UMathGenerator);
UMathGenerator::~UMathGenerator() {}
// ********** End Class UMathGenerator *************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MathGenerator_h__Script_OrbitalDecay_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMathQuestion::StaticStruct, Z_Construct_UScriptStruct_FMathQuestion_Statics::NewStructOps, TEXT("MathQuestion"),&Z_Registration_Info_UScriptStruct_FMathQuestion, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMathQuestion), 2920429780U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMathGenerator, UMathGenerator::StaticClass, TEXT("UMathGenerator"), &Z_Registration_Info_UClass_UMathGenerator, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMathGenerator), 3398773769U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MathGenerator_h__Script_OrbitalDecay_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MathGenerator_h__Script_OrbitalDecay_3690127849{
	TEXT("/Script/OrbitalDecay"),
	Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MathGenerator_h__Script_OrbitalDecay_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MathGenerator_h__Script_OrbitalDecay_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MathGenerator_h__Script_OrbitalDecay_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Lucas_OneDrive_Documents_GitHub_OrbitalDecay_OrbitalDecay_Source_OrbitalDecay_MathGenerator_h__Script_OrbitalDecay_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
