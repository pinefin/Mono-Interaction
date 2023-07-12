#pragma once
#include <Windows.h>
#include <cstdint>

#define ENCRYPT_STR //E //Just put whatever you use to encrypt strings, whether its a single letter macro or a function.
#define CREATE_STR(TXT) #TXT //Creates it to a string, usually used when you concatenate.

#define GET_MODULE_BASE(DLL_NAME) \
	if (!Mono::MonoDll) \
		Mono::MonoDll = (uint64_t)GetModuleHandleA(ENCRYPT_STR(DLL_NAME)); //Change to your own custom way to get module bases.
#define GET_MONO_EXPORT(EXPORT_NAME) GetProcAddress((HMODULE)Mono::MonoDll, EXPORT_NAME) //Change to your own custom function to get exports.

//Defines the export.
#define DEFINE_MONO_FN(name, args) \
	using name##_t = args;\
	name##_t name;

//Populates the variable we defined with the function address.
#define INIT_MONO_FN(name) \
	GET_MODULE_BASE("mono-2.0-bdwgc.dll"); \
	if (Mono::name == 0) \
		Mono::name = reinterpret_cast<Mono::name##_t>(GET_MONO_EXPORT(ENCRYPT_STR(CREATE_STR(mono_##name)))); 

class Mono_Signature;
class Mono_Domain;
class Mono_Class;
class Mono_Assembly;
class Mono_Image;
class Mono_Method;
class Mono_TableInfo;
class Mono_Vtable;
class Mono_Type;

namespace Mono {
	uint64_t MonoDll; //mono-2.0-bdwgc.dll

	//These names should not change as it uses the names to get the exports.
	DEFINE_MONO_FN(get_root_domain, Mono_Domain* (*)(void));
	DEFINE_MONO_FN(object_new, uint64_t(*)(Mono_Domain* domain, Mono_Class* klass));
	DEFINE_MONO_FN(thread_attach, void(*)(Mono_Domain* domain));
	DEFINE_MONO_FN(domain_assembly_open, Mono_Assembly* (*)(Mono_Domain* domain, const char* name));
	DEFINE_MONO_FN(assembly_get_image, Mono_Image* (*)(Mono_Assembly* assembly));
	DEFINE_MONO_FN(class_from_name, Mono_Class* (*)(Mono_Image* image, const char* name_space, const char* klass));
	DEFINE_MONO_FN(class_get_method_from_name, Mono_Method* (*)(Mono_Class* klass, const char* name, __int64 arg_count));
	DEFINE_MONO_FN(compile_method, uint64_t(*)(Mono_Method* method));
	DEFINE_MONO_FN(image_get_table_info, Mono_TableInfo* (*)(Mono_Image* image, int table_id));
	DEFINE_MONO_FN(table_info_get_rows, int64_t(*)(Mono_TableInfo* table_info));
	DEFINE_MONO_FN(class_get, Mono_Class* (*)(Mono_Image* image, unsigned int type_token));
	DEFINE_MONO_FN(class_get_name, const char* (*)(Mono_Class* klass));
	DEFINE_MONO_FN(class_get_namespace, const char* (*)(Mono_Class* klass));
	DEFINE_MONO_FN(class_num_methods, int64_t(*)(Mono_Class* klass));
	DEFINE_MONO_FN(class_get_methods, Mono_Method* (*)(Mono_Class* klass, uint64_t* iter));
	DEFINE_MONO_FN(method_get_name, const char* (*)(Mono_Method* method));
	DEFINE_MONO_FN(type_get_object, uint64_t(*)(Mono_Domain* domain, uint64_t klass));
	DEFINE_MONO_FN(class_get_type, uint64_t(*)(uint64_t klass));
	DEFINE_MONO_FN(method_signature, Mono_Signature* (*)(Mono_Method* method));
	DEFINE_MONO_FN(signature_get_param_count, int64_t(*)(Mono_Signature* signature));
	//DEFINE_MONO_FN(array_new, uint64_t(*)(Mono_Domain* domain, uint64_t type, __int64 size));
	//DEFINE_MONO_FN(get_byte_class, uint64_t(*)(void));
	//DEFINE_MONO_FN(array_addr_with_size, uint64_t(*)(uint64_t arr, int, __int64 pos));
	DEFINE_MONO_FN(object_get_class, Mono_Class* (*)(uint64_t object));
	DEFINE_MONO_FN(class_get_parent, Mono_Class* (*)(Mono_Class* klass));
	DEFINE_MONO_FN(class_vtable, Mono_Vtable* (*)(Mono_Domain* domain, Mono_Class* klass));
	DEFINE_MONO_FN(vtable_get_static_field_data, uint64_t(*)(Mono_Vtable* vtable));

	enum class Mono_MetaTableEnum {
		MONO_TABLE_MODULE,
		MONO_TABLE_TYPEREF,
		MONO_TABLE_TYPEDEF,
		MONO_TABLE_FIELD_POINTER,
		MONO_TABLE_FIELD,
		MONO_TABLE_METHOD_POINTER,
		MONO_TABLE_METHOD,
		MONO_TABLE_PARAM_POINTER,
		MONO_TABLE_PARAM,
		MONO_TABLE_INTERFACEIMPL,
		MONO_TABLE_MEMBERREF, /* 0xa */
		MONO_TABLE_CONSTANT,
		MONO_TABLE_CUSTOMATTRIBUTE,
		MONO_TABLE_FIELDMARSHAL,
		MONO_TABLE_DECLSECURITY,
		MONO_TABLE_CLASSLAYOUT,
		MONO_TABLE_FIELDLAYOUT, /* 0x10 */
		MONO_TABLE_STANDALONESIG,
		MONO_TABLE_EVENTMAP,
		MONO_TABLE_EVENT_POINTER,
		MONO_TABLE_EVENT,
		MONO_TABLE_PROPERTYMAP,
		MONO_TABLE_PROPERTY_POINTER,
		MONO_TABLE_PROPERTY,
		MONO_TABLE_METHODSEMANTICS,
		MONO_TABLE_METHODIMPL,
		MONO_TABLE_MODULEREF, /* 0x1a */
		MONO_TABLE_TYPESPEC,
		MONO_TABLE_IMPLMAP,
		MONO_TABLE_FIELDRVA,
		MONO_TABLE_UNUSED6,
		MONO_TABLE_UNUSED7,
		MONO_TABLE_ASSEMBLY, /* 0x20 */
		MONO_TABLE_ASSEMBLYPROCESSOR,
		MONO_TABLE_ASSEMBLYOS,
		MONO_TABLE_ASSEMBLYREF,
		MONO_TABLE_ASSEMBLYREFPROCESSOR,
		MONO_TABLE_ASSEMBLYREFOS,
		MONO_TABLE_FILE,
		MONO_TABLE_EXPORTEDTYPE,
		MONO_TABLE_MANIFESTRESOURCE,
		MONO_TABLE_NESTEDCLASS,
		MONO_TABLE_GENERICPARAM, /* 0x2a */
		MONO_TABLE_METHODSPEC,
		MONO_TABLE_GENERICPARAMCONSTRAINT
	};

	enum Mono_TokenType {
		MONO_TOKEN_MODULE = 0x00000000,
		MONO_TOKEN_TYPE_REF = 0x01000000,
		MONO_TOKEN_TYPE_DEF = 0x02000000,
		MONO_TOKEN_FIELD_DEF = 0x04000000,
		MONO_TOKEN_METHOD_DEF = 0x06000000,
		MONO_TOKEN_PARAM_DEF = 0x08000000,
		MONO_TOKEN_INTERFACE_IMPL = 0x09000000,
		MONO_TOKEN_MEMBER_REF = 0x0a000000,
		MONO_TOKEN_CUSTOM_ATTRIBUTE = 0x0c000000,
		MONO_TOKEN_PERMISSION = 0x0e000000,
		MONO_TOKEN_SIGNATURE = 0x11000000,
		MONO_TOKEN_EVENT = 0x14000000,
		MONO_TOKEN_PROPERTY = 0x17000000,
		MONO_TOKEN_MODULE_REF = 0x1a000000,
		MONO_TOKEN_TYPE_SPEC = 0x1b000000,
		MONO_TOKEN_ASSEMBLY = 0x20000000,
		MONO_TOKEN_ASSEMBLY_REF = 0x23000000,
		MONO_TOKEN_FILE = 0x26000000,
		MONO_TOKEN_EXPORTED_TYPE = 0x27000000,
		MONO_TOKEN_MANIFEST_RESOURCE = 0x28000000,
		MONO_TOKEN_GENERIC_PARAM = 0x2a000000,
		MONO_TOKEN_METHOD_SPEC = 0x2b000000,

		/*
		 * These do not match metadata tables directly
		 */
		 MONO_TOKEN_STRING = 0x70000000,
		 MONO_TOKEN_NAME = 0x71000000,
		 MONO_TOKEN_BASE_TYPE = 0x72000000
	};
}

class Mono_Domain {
public:
	//Attaches you to the mono thread to safely call mono functions.
	void AttachThread() {
		INIT_MONO_FN(thread_attach);
		return Mono::thread_attach(this);
	}

	//Open assembly module
	Mono_Assembly* OpenAssembly(const char* name) {
		INIT_MONO_FN(domain_assembly_open);
		return Mono::domain_assembly_open(this, name);
	}

	//You should usually use a pointer if you make a class with it, otherwise just use uint64_t (or any other 64 bit variable).
	template <typename T>
	T NewObject(Mono_Class* klass) {
		INIT_MONO_FN(object_new);
		return Mono::object_new(this, klass);
	}

	static Mono_Domain* GetRootDomain() {
		INIT_MONO_FN(get_root_domain);
		return Mono::get_root_domain();
	}
};

class Mono_Signature
{
public:
	//Get the parameter count of the signature, used for methods.
	int64_t GetParameterCount() {
		if (!this)
			return 0;
		INIT_MONO_FN(signature_get_param_count);
		return Mono::signature_get_param_count(this);
	}
};

class Mono_TableInfo {
public:
	int64_t GetRows() {
		INIT_MONO_FN(table_info_get_rows);
		return Mono::table_info_get_rows(this);
	}
};

class Mono_Method {
public:
	//Get the parameter count of the method.
	inline int64_t GetParameterCount() {
		return this->GetSignature()->GetParameterCount();
	}

	//Gets the signature of the function. No it's not like a signature like sig-scanning.
	Mono_Signature* GetSignature() {
		INIT_MONO_FN(method_signature);
		return Mono::method_signature(this);
	}

	const char* GetName() {
		INIT_MONO_FN(method_get_name);
		return Mono::method_get_name(this);
	}

	//Pass the type to the function to get the function type instead of having to cast afterwards.
	template <typename T = uint64_t>
	T CompileMethod() {
		if (!this)
			return 0;
		INIT_MONO_FN(compile_method);
		return (T)Mono::compile_method(this);
	}
};

class Mono_Class {
public:
	//Get namespace name
	const char* GetNamespaceName() {
		INIT_MONO_FN(class_get_namespace);
		return Mono::class_get_namespace(this);
	}

	//Get class name
	const char* GetName() {
		INIT_MONO_FN(class_get_name);
		return Mono::class_get_name(this);
	}

	//Get class parent
	Mono_Class* GetParentClass() {
		INIT_MONO_FN(class_get_parent);
		return Mono::class_get_parent(this);
	}

	//Get method count in class
	int64_t GetMethodCount() {
		INIT_MONO_FN(class_num_methods);
		return Mono::class_num_methods(this);
	}

	//Iterate methods with this
	Mono_Method* IterateMethods(uint64_t* iter) {
		INIT_MONO_FN(class_get_methods);
		return Mono::class_get_methods(this, iter);
	}

	//Get the classes vtable.
	Mono_Vtable* GetVtable() {
		if (!this)
			return 0;

		Mono_Domain* Domain = Mono_Domain::GetRootDomain();
		if (!Domain)
			return 0;

		INIT_MONO_FN(class_vtable);
		return Mono::class_vtable(Domain, this);
	}

	//Get method from name, this does not include functions that have obfuscated names.
	//Remember to use -1 if you don't want to use ArgCount parameter.
	Mono_Method* GetMethod(const char* MethodName, __int64 ArgCount = -1) {
		INIT_MONO_FN(class_get_method_from_name);
		return Mono::class_get_method_from_name(this, MethodName, ArgCount);
	}

	template <typename T = uint64_t>
	T GetCompiledMethod(const char* MethodName, int ArgCount = -1) {
		if (!this)
			return (T)0;
		auto Method = this->GetMethod(MethodName, ArgCount);
		if (!Method)
			return (T)0;
		return Method->CompileMethod<T>();
	}
};

class Mono_Image {
public:
	//Get table info from table id.
	Mono_TableInfo* GetTableInfo(int TableId) {
		INIT_MONO_FN(image_get_table_info);
		return Mono::image_get_table_info(this, TableId);
	}

	//Get class from type token
	Mono_Class* GetClass(int TypeToken) {
		INIT_MONO_FN(class_get);
		return Mono::class_get(this, TypeToken);
	}

	//Get Class from name.
	Mono_Class* GetClassFromName(const char* Namespace_Name, const char* Class_Name) {
		if (!this)
			return 0;

		INIT_MONO_FN(class_from_name);
		return Mono::class_from_name(this, Namespace_Name, Class_Name);
	}
};

class Mono_Assembly {
public:
	//Get assembly image
	Mono_Image* GetImage() {
		if (!this)
			return 0;
		INIT_MONO_FN(assembly_get_image);
		return Mono::assembly_get_image(this);
	}
};

class Mono_Object {
public:
	//Get object class
	static Mono_Class* GetClass(uint64_t object) {
		INIT_MONO_FN(object_get_class);
		return Mono::object_get_class(object);
	}
};

class Mono_Vtable {
public:
	template <typename T = uint64_t>
	T GetStaticFieldData() {
		INIT_MONO_FN(vtable_get_static_field_data);
		return (T)Mono::vtable_get_static_field_data(this);
	}
};