// Copyright 2023 CQUnreal. All Rights Reserved.

package com.epicgames.ue4;

import com.google.gson.Gson;

public class OAuthLogin
{
	/** OAuthLogin Responses supported */
	public static final int RESPONSE_SUCCESS = 0;
	public static final int RESPONSE_CANCELED = 1;
	public static final int RESPONSE_NOT_SUPPORTED = 2;
	public static final int RESPONSE_SYSTEM_ERROR = 3;

	/** AntiAddiction Event supported */
	public static final int ANTI_ADDICTION_INIT = 0;

	public static String makeOAuthInitData(int code)
	{
		return "{ \"Code\": \"" + code + "\"}";
	}

	public static String makeOAuthLoginData(int code, OAuthData oauthData)
	{	
		Gson gson = new Gson();
		return "{ \"Code\": \"" + code + "\", \"OAuthData\": " + gson.toJson(oauthData) + " }";
	}

	public static String makeOAuthLogoutData(int code)
	{
		return "{ \"Code\": \"" + code + "\"}";
	}
	
	public static String makeAntiAddictionEventData(int event, int code, AntiAddictionData antiAddictionData)
	{
		Gson gson = new Gson();
		return "{ \"Event\": \"" + event + "\", \"Code:\" \"" + code + "\", \"AntiAddictionData\": " + gson.toJson(antiAddictionData) + " }";
	}
}