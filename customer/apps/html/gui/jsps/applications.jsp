<%@page pageEncoding="UTF-8"%>
<%@ page import="java.io.*" %>
<%@ page import="java.nio.charset.*" %>
<%@ page import="org.json.*" %>
<%
String basePath = request.getRealPath("/");

String returnApplicationsJSON = request.getParameter("JSON");

String userAgent = request.getHeader("user-agent");
if (userAgent.indexOf("MSIE") > -1) {
   returnApplicationsJSON = new String(returnApplicationsJSON.getBytes("ISO-8859-1"), "UTF-8" );
}
String returnApplicationsListJSON ="";
try { 
	JSONObject jsonApplications = new JSONObject(returnApplicationsJSON);
	JSONArray appList = (JSONArray) jsonApplications.get("APPLICATIONS");
	
	for(int i=0; i< appList.length(); i++){
		JSONObject appObject = appList.getJSONObject(i);
		appObject.remove("RULES");
	}
	returnApplicationsListJSON="{\"RETURN\":{\"success\": true},\"APPLICATION_LIST\":"+appList+"}";
	
}catch (Exception e){
	out.println("{\"RETURN\":{\"success\": false, \"errorDescription\":\"ERROR_GENERAL\"}");
}
	
try { 
		FileOutputStream file_out = new FileOutputStream(basePath+"APIS/returnApplicationsListJSON.txt"); 
		file_out.write(		returnApplicationsListJSON.getBytes("UTF-8") ); 
		file_out.close();
				
	} catch(IOException ioe) { 
		out.println("{\"RETURN\":{\"success\": false, \"errorDescription\":\"ERROR_GENERAL\"}");

	}
	try { 
		FileOutputStream file_out = new FileOutputStream(basePath+"APIS/returnApplicationsJSON.txt"); 
		file_out.write(		returnApplicationsJSON.getBytes("UTF-8") ); 
		file_out.close();
	
	} catch(IOException ioe) { 
		out.println("{\"RETURN\":{\"success\": false, \"errorDescription\":\"ERROR_GENERAL\"}");
	
	}
		
%>
<%=returnApplicationsJSON %>
