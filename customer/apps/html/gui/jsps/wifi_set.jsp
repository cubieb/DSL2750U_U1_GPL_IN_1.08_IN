<%@ page pageEncoding="UTF-8"%>
<%@ page import="java.io.*" %>
<%@ page import="java.nio.charset.*" %>
<%@ page import="org.json.*" %>
<%
String jsonParameter = request.getParameter("JSON");

String filepath = "APIS/returnWifiJSON.txt";
String basePath = request.getRealPath("/");

String browserType=(String)request.getHeader("User-Agent");

boolean firefox = true;
if (browserType.indexOf("MSIE") > -1) firefox=false;

byte[] buffer = new byte[2048];
try { 
	//READING ACTUAL DATA
	FileInputStream file = new FileInputStream(basePath+filepath); 
	int n = file.read(buffer); 
	file.close();

	String jsonstring = new String(buffer,0,n,"UTF-8");
	if (!firefox) 	jsonParameter = new String(	jsonParameter.getBytes("ISO-8859-1"), "UTF-8" );

	JSONObject original = new JSONObject(jsonstring);
	JSONObject new_data = new JSONObject(jsonParameter);
	if (new_data.has("WIFI"))
	{
		JSONObject original_wifi = (JSONObject)original.get("WIFI");
		JSONObject nuevo_wifi = (JSONObject)new_data.get("WIFI");
		if (nuevo_wifi.has("status"))
			original_wifi.put("status", nuevo_wifi.getBoolean("status"));
		if (nuevo_wifi.has("ssidName"))
			original_wifi.put("ssidName", nuevo_wifi.getString("ssidName"));
		if (nuevo_wifi.has("ssidVisibility"))
			original_wifi.put("ssidVisibility", nuevo_wifi.getBoolean("ssidVisibility"));
		if (nuevo_wifi.has("channelMode"))
			original_wifi.put("channelMode", nuevo_wifi.getString("channelMode"));
		if (nuevo_wifi.has("channel"))
			original_wifi.put("channel", nuevo_wifi.getString("channel"));
		if (nuevo_wifi.has("wifiAdministration"))
			original_wifi.put("wifiAdministration", nuevo_wifi.getString("wifiAdministration"));
		
		if (nuevo_wifi.has("SECURITY"))
		{
			JSONObject security_old = (JSONObject)original_wifi.get("SECURITY");
			JSONObject security_new = (JSONObject)nuevo_wifi.get("SECURITY");
			
			if (security_new.has("cipherAlgorithm"))
				security_old.put("cipherAlgorithm", security_new.getString("cipherAlgorithm"));
			if (security_new.has("algVersion"))
				security_old.put("algVersion", security_new.getString("algVersion"));
			if (security_new.has("passwordWEP"))
				security_old.put("passwordWEP", security_new.getString("passwordWEP"));
			if (security_new.has("passwordWPA"))
				security_old.put("passwordWPA", security_new.getString("passwordWPA"));
			if (security_new.has("passwordWPA2"))
				security_old.put("passwordWPA2", security_new.getString("passwordWPA2"));
		}
	}
	
	if (new_data.has("DHCP"))
	{
		JSONObject original_dhcp = (JSONObject)original.get("DHCP");
		JSONObject new_dhcp = (JSONObject)new_data.get("DHCP");
		
		if (new_dhcp.has("status"))
			original_dhcp.put("status", new_dhcp.getBoolean("status"));
		if (new_dhcp.has("poolStart"))
			original_dhcp.put("poolStart", new_dhcp.getString("poolStart"));
		if (new_dhcp.has("poolEnd"))
			original_dhcp.put("poolEnd", new_dhcp.getString("poolEnd"));
	}
	if (new_data.has("LAN")){
		JSONObject original_lan = (JSONObject)original.get("LAN");
		JSONObject new_lan = (JSONObject)new_data.get("LAN");
		if (new_lan.has("ip"))
			original_lan.put("ip", new_lan.getString("ip"));
		if (new_lan.has("mask"))
			original_lan.put("mask", new_lan.getString("mask"));
	}
	
	//SAVING CHANGES
	
	FileOutputStream file_out = new FileOutputStream(basePath+filepath); 
	file_out.write(		original.toString().getBytes("UTF-8") ); 
	file_out.close();
	
} catch(IOException ioe) { 
	// Error
	System.out.println("Error");
	System.out.println(ioe);
}

%>{
	"RETURN":{
		"success": true
		,"errorDescription":"ERROR_CHANGE_PASS"
      }
}