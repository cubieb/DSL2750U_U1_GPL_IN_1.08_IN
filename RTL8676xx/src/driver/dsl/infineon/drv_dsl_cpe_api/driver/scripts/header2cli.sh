INPATH=../src/include
OUTPATH=../src/cli
OUTFILE=drv_dsl_cpe_cli
OUTFILE_CLI=../src/cli/drv_dsl_cpe_cli
SWIG=swig
XSLTPROC=xsltproc


echo calling SWIG, generate from interface header the xml file
swig -o ./$OUTFILE.xml -ignoremissing -includeall -xml -module dsl_cpe -I$INPATH drv_dsl_cpe_api.h

echo calling xsltproc, transform the xml file into the command line interface file
xsltproc -o $OUTFILE_CLI.c ./swig2cli.xsl ./$OUTFILE.xml
#Xalan -o $OUTFILE_CLI.c ./$OUTFILE.xml ./swig2cli.xsl

sed -i 's/IFX_/DSL_/g' $OUTFILE_CLI.c

echo calling indent, bring the c coding into the correct form
indent $OUTFILE_CLI.c

