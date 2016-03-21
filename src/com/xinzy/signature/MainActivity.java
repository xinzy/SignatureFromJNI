package com.xinzy.signature;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.xinzy.signature.util.Fingerprint;

public class MainActivity extends Activity implements OnClickListener {

	private TextView mTextView;
	private Button mButton;

	private Fingerprint mFinger;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mTextView = (TextView) findViewById(R.id.signText);
		mButton = (Button) findViewById(R.id.signButton);
		mButton.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {

		switch (v.getId()) {
		case R.id.signButton:
			mFinger = new Fingerprint();
			String str = "from jni: " + mFinger.getSign(this) + "\n";
			str += "from java: " + getSign();
			mTextView.setText(str);
			break;

		default:
			break;
		}
	}

	private String getSign() {
		try {
			PackageInfo info = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
			if (info.signatures != null && info.signatures.length > 0) {
				Signature signature = info.signatures[0];
				return md5(signature.toByteArray());
			}
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return null;
	}

	private String md5(byte[] input) {

		char[] hexDigits = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		try {
			MessageDigest messageDigest = MessageDigest.getInstance("MD5");
			messageDigest.update(input);
			byte[] digest = messageDigest.digest();

			char[] resultCharArray = new char[digest.length * 2];
			int index = 0;
			for (byte b : digest) {

				resultCharArray[index++] = hexDigits[b >>> 4 & 0xf];
				resultCharArray[index++] = hexDigits[b & 0xf];
			}
			return new String(resultCharArray);
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}

		return null;
	}
}
