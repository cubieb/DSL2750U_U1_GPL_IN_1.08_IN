<%@ page import="java.io.*" %>
<%@page pageEncoding="UTF-8"%>
<%@ page import="java.nio.charset.*" %>
<%String operation = request.getParameter("operation");
String JSONData=request.getParameter("JSON");

String userAgent = request.getHeader("user-agent");
if (userAgent.indexOf("MSIE") > -1) {
	JSONData = new String(JSONData.getBytes("ISO-8859-1"), "UTF-8" );
}
FileOutputStream file = null;
String basePath = request.getRealPath("/");

if (!operation.equalsIgnoreCase("modifyListAssignedApp")){
	try { 
		file = new FileOutputStream(basePath+"APIS/returnDevicesJSON.txt"); 
		file.write(JSONData.getBytes("UTF-8") ); 
		file.close();
	
	} catch(IOException ioe) { 
		out.println("{\"RETURN\":{\"success\": false, \"errorDescription\":\"ERROR_GENERAL\"}");
		return;
	}
}else{
	try { 
		file = new FileOutputStream(basePath+"APIS/returnApplicationsListJSON.txt"); 
		file.write(JSONData.getBytes("UTF-8") ); 
		file.close();
	} catch(IOException ioe) { 
		out.println("{\"RETURN\":{\"success\": false, \"errorDescription\":\"ERROR_GENERAL\"}");
		return;
	}
}

out.println(JSONData);
%>