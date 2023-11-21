package com.tds;

import android.app.Activity;
import android.os.Handler;
import android.os.Looper;

import com.tapsdk.antiaddictionui.AntiAddictionUIKit;

public class AntiAddictionUE {
    public static void startupWithTapTap(Activity activity, String userIdentifier) {
        Handler mainHandler = new Handler(Looper.getMainLooper());
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                AntiAddictionUIKit.startupWithTapTap(activity, userIdentifier);
            }
        });
    }
    
    public static void setTestEnv(Activity activity, boolean enable) {
        Handler mainHandler = new Handler(Looper.getMainLooper());
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                AntiAddictionUIKit.setTestEnvironment(activity, enable);
            }
        });
    }
}
