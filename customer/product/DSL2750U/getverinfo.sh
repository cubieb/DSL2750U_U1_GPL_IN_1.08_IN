#!/bin/sh
version="GAN9.9T113A-B-DL-DSL2750U-R5B0017-India.EN_2T2R"
auth=""
lastrev=""
revURL=""

flashList="	
	{AT25DF321   ,0x1F4700},
	{AT25DF321   ,0x1F4701},
	{A25L032     ,0x373016},
	{EN25P32     ,0x1C2016},
	{EN25F32     ,0x1C3116},
	{EN25Q32     ,0x1C3316},
	{EN25Q32A    ,0x1C3016},
	{M25P32      ,0x202016},
	{MX25L3205D  ,0xc22016},
	{MX25L3235D  ,0xc22416},
	{N25S32      ,0xD53016},
	{W25X32      ,0xEF3016},
	{W25Q32      ,0xEF4016},
	{S25FL032A   ,0x010215},
	{GD25Q32     ,0xC84016},
	{AT25DF641   ,0x1F4800},
	{A25L064     ,0x373017},
	{EN25P64     ,0x1C2017},
	{EN25Q64     ,0x1C3017},
	{M25P64      ,0x202017},
	{MX25L6405D  ,0xc22017},
	{W25X64      ,0xEF3017},
	{W25Q64      ,0xEF4017},
	{S25FL064A   ,0x010216},
	{GD25Q64     ,0xC84017},
	{GD25Q128    ,0xC84018},
	{AT25DF128   ,0x1F4900},
	{A25L128     ,0x373018},
	{EN25P128    ,0x1C2018},
	{EN25Q128    ,0x1C3018},
	{N25Q128     ,0x20BA18},
	{N25Q128     ,0x20BB18},
	{MX25L12805  ,0xc22018},
	{W25X128     ,0xEF3018},
	{W25Q128     ,0xEF4018},
	{S25FL128P   ,0x012018}"

LANGINIT=`echo $LANG`

export LANG="zh_CN.UTF-8"
cd ../
svn info > temp
if [ $? -eq 0 ]; then
	lastrev=`cat temp | sed -n 's/最后修改的版本: //gp'`
	if [ -z "$lastrev" ]; then
	      lastrev=`cat temp | sed -n 's/Last Changed Rev: //gp'`
	fi
	revURL=`cat temp | sed -n 's/URL: //gp'`
else
	lastrev=`echo "None"`
	revURL=`echo "None"`
fi

rm temp -rf
auth=`who am i`
auth=`echo ${auth%pts*}`

export LANG=$LANGINIT
cd -
./hidenPage.sh "$version" "$auth" "$lastrev" "$revURL" "$flashList"
