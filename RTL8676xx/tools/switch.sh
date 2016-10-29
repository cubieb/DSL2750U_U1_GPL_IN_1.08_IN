#!/bin/sh
OLDPWD=${PWD}
if [ "-${ROOTDIR}" == "-" ]; then
  echo "Please set environ variable ROOTDIR"
  exit 1
fi
cd ${ROOTDIR}
index=1;
echo "*************************************************"
echo "* Please select a product:"
ProductList=`ls -1 -d build.* | cut -c 7-`
BuildLink=`ls -ld build`
ActiveBuild=`expr match "$BuildLink" ".*build\.\([^\/ ]*\)"`
for ProductName in ${ProductList}; do
  if [ "${ActiveBuild}" = "${ProductName}" ]; then
	Actived=" + "
  else
	Actived="   "
  fi
  echo "* ${Actived} $index: $ProductName"
  eval VAR$index='$ProductName'
  index=`expr $index + 1`
done 
echo "*************************************************"
read item;
eval "Product=\"\$VAR${item}\""
export PRODUCT=`echo ${Product}|awk -F'__' '{print $1}'` 
export PROFILE=`echo ${Product}|awk -F'__' '{print $2}'`

rm -f build
ln -s build.${Product} build

echo "Switch product from ${ActiveBuild} to ${Product} successed!"
cd ${OLDPWD}

