<%@page pageEncoding="UTF-8"%>
<%@ page import="java.io.*" %>
<%@ page import="org.json.*" %>
<%
String filepath = "APIS/returnInternetJSON.txt";
String basePath = request.getRealPath("/");
byte[] buffer = new byte[2048];
boolean error =  false;

try { 
	FileInputStream file = new FileInputStream(basePath+filepath); 
	int n = file.read(buffer); 
	file.close();
	
	String jsonstring = new String(buffer,0,n,"UTF-8");
	JSONObject object = new JSONObject(jsonstring);
	
	
	JSONObject return_obj = new JSONObject();
	return_obj.put("success", new Boolean(!error));
	if (error) return_obj.put("errorDescription", "ERROR_CHANGE_PASS");
	object.put("RETURN",return_obj);
		
%><%= object%><%
} 
catch(IOException ioe) {}
%>