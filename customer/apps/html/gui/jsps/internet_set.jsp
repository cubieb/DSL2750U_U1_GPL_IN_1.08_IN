<%@ page pageEncoding="UTF-8"%>
<%@ page import="java.io.*" %>
<%@ page import="java.nio.charset.*" %>
<%@ page import="org.json.*" %>
<%
String jsonParameter = request.getParameter("JSON");

String filepath = "APIS/returnInternetJSON.txt";
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
//System.out.println(jsonParameter);
	JSONObject original = new JSONObject(jsonstring);
	JSONObject new_data = new JSONObject(jsonParameter);
	
	if (new_data.has("INTERNET"))
	{
		JSONObject original_int = (JSONObject)original.get("INTERNET");
		JSONObject nuevo_int = (JSONObject)new_data.get("INTERNET");
		if (nuevo_int.has("PPP")){
			JSONObject original_PPP = (JSONObject)original_int.get("PPP");
			JSONObject nuevo_PPP = (JSONObject)nuevo_int.get("PPP");
			if (nuevo_PPP.has("username")){
				original_PPP.put("username", nuevo_PPP.getString("username"));
			}
				
			if (nuevo_PPP.has("password")){
				original_PPP.put("password", nuevo_PPP.getString("password"));
			}
		}
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