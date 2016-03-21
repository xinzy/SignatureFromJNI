package com.xinzy.signature.util;

import android.content.Context;

public class Fingerprint {

	static {
		System.loadLibrary("finger");
	}
	
	public native String getSign(Context context);
	
}
