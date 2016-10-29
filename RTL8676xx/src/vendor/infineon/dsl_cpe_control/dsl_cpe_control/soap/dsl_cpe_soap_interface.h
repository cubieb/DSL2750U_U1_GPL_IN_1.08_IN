//gsoap ifx service name:	   dslapi
//gsoap ifx service style:	   rpc
//gsoap ifx service encoding:	encoded
//gsoap ifx service namespace:	urn:dsl_api
//gsoap ifx service location:	http://board:8080

//gsoap ifx service method-action: DslCliAccess ""

typedef char* xsd__string;

int ifx__DslCpeCliAccess(xsd__string command, xsd__string *result);
int ifx__DslCpeEventCallback(xsd__string command, xsd__string *result);
