// Copyright 2023 CQUnreal. All Rights Reserved.

package com.epicgames.ue4;

public class OAuthLogin
{
	/** OAuthLogin Responses supported */
	public static final int RESPONSE_SUCCESS = 0;
	public static final int RESPONSE_CANCELED = 1;
	public static final int RESPONSE_NOT_SUPPORTED = 2;
	public static final int RESPONSE_SYSTEM_ERROR = 3;
	public static final int RESPONSE_NETWORK_ERROR = 4;

	/** AntiAddiction Event supported */
	public static final int ANTI_ADDICTION_STARTUP = 0;
	public static final int ANTI_ADDICTION_SHUTDOWN = 1;

	public static class OAuthData
	{	
		public String UID;
	
		public String Nickname;
	
		public String Avatar;
	
		public String Key;
	
		public String AccessToken;
	}

	public static class AntiAddictionData
	{
	
	}

	public static String makeOAuthInitData(int code)
	{
		return "{ \"Code\": " + code + " }";
	}

	public static String makeOAuthLoginData(int code, OAuthData data)
	{	
		String dataJson = "{}";
		if (data != null)
		{
			dataJson = "{ \"UID\": \"" + data.UID + "\", " +
				"\"Nickname\": \"" + data.Nickname + "\", " +
				"\"Avatar\": \"" + data.Avatar + "\", " +
				"\"Key\": \"" + data.Key + "\", " +
				"\"AccessToken\": \"" + data.AccessToken + "\"" +
			"}";
		}
		return "{ \"Code\": " + code + ", \"Data\": " + dataJson + " }";
	}

	public static String makeOAuthLogoutData(int code)
	{
		return "{ \"Code\": " + code + " }";
	}
	
	public static String makeAntiAddictionEventData(int event, int code, AntiAddictionData data)
	{	
		String dataJson = "{}";
		if (data != null)
		{
			dataJson = "{}";
		}
		return "{ \"Event\": " + event + ", \"Code\": " + code + ", \"Data\": " + dataJson + " }";
	}
}