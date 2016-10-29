import sys

from SOAPpy import WSDL, SOAPProxy

wsdl='dslcpeapi.wsdl'


try:
  cmd = sys.argv[1]
except IndexError:
  cmd = "help all"
  
server = WSDL.Proxy(wsdl)

methods = server.methods.keys()

print "Available methods: "
for k in methods:
   print k
   print "   Input parameter:"
   for arg in server.methods[k].inparams:
      print "   - ", arg.name.ljust(8), arg.type
   print "   Output parameter:"
   for arg in server.methods[k].outparams:
      print "   - ", arg.name.ljust(8), arg.type
   print ""      


#res = server.DslCliAccess(command=cmd)
res = server.DslCliAccess(command="lsg")


print res

 
