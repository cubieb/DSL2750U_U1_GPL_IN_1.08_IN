echo "##################################"
echo "#  Processing  DSL CPE API V1.x  #"
echo "##################################"

echo "build Danube for mips ..."
./build/build_danube_mips.sh

echo "strip Danube for mips ..."
./build/strip_danube_mips.sh

echo "copy binary (drv_dsl_cpe_api) to NFS folder (/exports/nfs_danube/opt/ifx/bin) ..."
cp ./build_danube/src/drv_dsl_cpe_api /exports/nfs_danube/opt/ifx/bin

