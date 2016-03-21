#include "com_xinzy_signature_util_Fingerprint.h"

void byteToHex(const unsigned char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;

        highByte += 0x30;

        if (highByte > 0x39)
                dest[i * 2] = highByte + 0x07;
        else
                dest[i * 2] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
}

JNIEXPORT jstring JNICALL JNI_FN(getSign)(JNIEnv *env, jclass obj, jobject context)
{
	//Context的类
	jclass contextClass = (*env)->GetObjectClass(env, context);
	// 得到 getPackageManager 方法的 ID
	jmethodID getPackageManagerMethod = (*env)->GetMethodID(env, contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");

	// 获得PackageManager对象
	jobject packageManager = (*env)->CallObjectMethod(env, context, getPackageManagerMethod);
	// 获得 PackageManager 类
	jclass packageManagerClass = (*env)->GetObjectClass(env, packageManager);

	// 得到 getPackageName 方法的 ID
	jmethodID getPackageNameMethod = (*env)->GetMethodID(env, contextClass, "getPackageName", "()Ljava/lang/String;");
	// 获得当前应用的包名
	jstring packageName = (*env)->CallObjectMethod(env, context, getPackageNameMethod);
	const char *str = (*env)->GetStringUTFChars(env, packageName, JNI_FALSE);
	LOGI("packageName: %s\n", str);

	// 得到 getPackageInfo 方法的 ID
	jmethodID getPackageInfoMethod = (*env)->GetMethodID(env, packageManagerClass, "getPackageInfo",
			"(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
	// 获得PackageInfo
	jobject packageInfo = (*env)->CallObjectMethod(env, packageManager, getPackageInfoMethod, packageName, 64);

	jclass packageInfoClass = (*env)->GetObjectClass(env, packageInfo);
	jfieldID signaturesField = (*env)->GetFieldID(env, packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
	jobjectArray signatureArray = (jobjectArray)(*env)->GetObjectField(env, packageInfo, signaturesField);

	jsize sizeOfSign = (*env)->GetArrayLength(env, signatureArray);
	//确认signatureArray 数组不为空
	if (sizeOfSign > 0)
	{
		//Signature数组中取出第一个元素
		jobject signature = (*env)->GetObjectArrayElement(env, signatureArray, 0);
		//读signature的hashcode
		jclass signatureClass = (*env)->GetObjectClass(env, signature);
		jmethodID hashcodeMethod = (*env)->GetMethodID(env, signatureClass, "hashCode", "()I");
		jint hashCode = (*env)->CallIntMethod(env, signature, hashcodeMethod);
		LOGI("hashcode: %d\n", hashCode);

		jmethodID toByteArrayMethod = (*env)->GetMethodID(env, signatureClass, "toByteArray", "()[B");
		// JNI中的数组在Java中都是Object, 拿到签名的byte数组数据
		jobject byteArray = (*env)->CallObjectMethod(env, signature, toByteArrayMethod);

		//对byte array 进行MD5
		jclass messageDigestClass = (*env)->FindClass(env, "java/security/MessageDigest");
		jmethodID instanceMethod = (*env)->GetStaticMethodID(env, messageDigestClass, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
		jobject messageDigest = (*env)->CallStaticObjectMethod(env, messageDigestClass, instanceMethod, (*env)->NewStringUTF(env, "MD5"));
		jmethodID updateMethod = (*env)->GetMethodID(env, messageDigestClass, "update", "([B)V");
		(*env)->CallVoidMethod(env, messageDigest, updateMethod, byteArray);
		jmethodID digestMethod = (*env)->GetMethodID(env, messageDigestClass, "digest", "()[B");
		jobject digestBytes = (*env)->CallObjectMethod(env, messageDigest, digestMethod);

		jsize arrayLength = (*env)->GetArrayLength(env, digestBytes);
		jbyte* byteElement = (*env)->GetByteArrayElements(env, digestBytes, JNI_FALSE);
		char* charResult = (char*) malloc(arrayLength * 2 + 1);// +1在后面一位补上\0
		byteToHex(byteElement, charResult, arrayLength);
		*(charResult + arrayLength * 2) = '\0';// 在末尾补\0
		LOGI("Sign md5: %s\n", charResult);

		jstring result = (*env)->NewStringUTF(env, charResult);
		free(charResult);

		return result;
	} else
	{
		return NULL;
	}
}
