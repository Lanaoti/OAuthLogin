#include "LCJavaHelper.h"
#include "Tools/LCJsonHelper.h"
#define JavaHelperUE "com/lc/JavaHelperUE"

LCJNI::Object LCJavaHelper::ConvertUEMap(const LCJNI::JNI& LCJNI, TMap<FString, double> UE_Map) {
	auto mapClass = LCJNI.FindClass("java/util/HashMap");
	auto hashMap = LCJNI.NewObject(mapClass, "()V");
	for (auto Map : UE_Map) {
		auto keyJava = LCJNI.ToJavaString(Map.Key);
		LCJNI.CallObjectMethod(hashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;", *keyJava, *LCJNI.GetJavaObject(Map.Value));
	}
	return hashMap;
}

LCJNI::Object LCJavaHelper::ConvertUEArray(const LCJNI::JNI& LCJNI, TArray<FString> UE_Array) {
	auto ArrayListClass = LCJNI.FindClass("java/util/ArrayList");
	auto list = LCJNI.NewObject(ArrayListClass, "()V");
	for (auto Array : UE_Array) {
		auto string = LCJNI.ToJavaString(Array);
		LCJNI.CallBooleanMethod(list, "add", "(Ljava/lang/Object;)Z", *string);
	}
	return list;
}


FString LCJavaHelper::GetJsonFromMap(const LCJNI::JNI& LCJNI, const LCJNI::Object& object) {
	auto Class = LCJNI.FindClass(JavaHelperUE);
	auto string = LCJNI.CallStaticObjectMethod(Class, "getJson", "(Ljava/util/Map;)Ljava/lang/String;", *object);
	return LCJNI.GetFString(string);
}

TSharedPtr<FLCObject> LCJavaHelper::ConvertObject(const LCJNI::Object& object) {
	if (!object) {
		return nullptr;
	}
	return MakeShared<FLCObject>(object);
}

TSharedPtr<FLCUser> LCJavaHelper::ConvertUser(const LCJNI::Object& user) {
	if (!user) {
		return nullptr;
	}
	return MakeShared<FLCUser>(user);
}

void LCJavaHelper::ListForEach(const LCJNI::JNI& LCJNI, const LCJNI::Object& listObject, TFunction<void(int Index, const LCJNI::Object& Object)> Block) {
	if (!listObject) {
		return;
	}
	int list_size = LCJNI.CallIntMethod(listObject, "size", "()I");
	for (int i = 0; i < list_size; i++) {
		auto obj = LCJNI.CallObjectMethod(listObject, "get", "(I)Ljava/lang/Object;", i);
		Block(i, obj);
	}
}