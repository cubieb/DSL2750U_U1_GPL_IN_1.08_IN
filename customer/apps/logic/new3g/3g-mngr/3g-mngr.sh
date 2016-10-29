#!/bin/bash

#set -x 

BIT_DEF_FILE="3g-mngr-bit.h"

function PROFILE_START() {
	local PROFILE=$1
	
	if [ ! -z $PROFILE ] && [ $PROFILE != "0" ]; then
		echo "#ifdef $PROFILE" 
	fi	
}

function PROFILE_END {
	local PROFILE=$1
	
	if [ ! -z $PROFILE ] && [ $PROFILE != "0" ]; then
	 	echo "#endif" 
	fi 	
}

function OBJ_MEMBER () {
#process OBJ_MEMBER(obj, m_type, m_name, bit, bit_name)
        local OBJ=$1
	local M_TYPE=$2
        local M_NAME=$3
        local BIT=$4
        local BIT_NAME=$5
        local PROFILE=$6

	local M_TYPE_NAME=$(echo $M_TYPE|sed 's/_t//')
	local M_VAR=$M_NAME
        local OBJ_NAME=$OBJ
	local OBJ_VAR=$OBJ_NAME
        local OBJ_TYPE=$OBJ_NAME"_t"

        local VAR_FILE=$OBJ"_var.c"
        local CODE_SAVE=$OBJ"_save.c"
        local CODE_RESTORE=$OBJ"_restore.c"
        local CODE_FREE=$OBJ"_free.c"
        local CODE_CREATE=$OBJ"_create.c"

        #create create function
        echo "">>$CODE_CREATE
        
        PROFILE_START $PROFILE>>$CODE_CREATE 
	 
        if [ ! -z $BIT  ] && [ ! -z $BIT_NAME ]  && [ $BIT_NAME != "0" ]; then
                echo "	if (($OBJ_VAR->who & $BIT_NAME) )">>$CODE_CREATE
                echo "		$OBJ_VAR->$M_NAME = $M_TYPE_NAME"_new"($OBJ_VAR);">>$CODE_CREATE
                echo "	else">>$CODE_CREATE
                echo "		$OBJ_VAR->$M_NAME = 0;">>$CODE_CREATE
		
		echo "#define $BIT_NAME (1<<$BIT)">>$BIT_DEF_FILE

        else
                echo "	$OBJ_VAR->$M_NAME = $M_TYPE_NAME"_new"($OBJ_VAR);">>$CODE_CREATE
        fi
        
	 PROFILE_END $PROFILE>>$CODE_CREATE     

        #create var 
	 PROFILE_START $PROFILE>>$VAR_FILE 
	 
        echo "	$M_TYPE *$M_VAR = 0;">>$VAR_FILE
        
	 PROFILE_END $PROFILE>>$VAR_FILE 

        #create save segment code file
	 PROFILE_START $PROFILE>>$CODE_SAVE         
        echo "	$M_VAR = $OBJ_VAR->$M_NAME;">>$CODE_SAVE
	 PROFILE_END $PROFILE>>$CODE_SAVE           

        #create restore segment code file
	PROFILE_START $PROFILE>>$CODE_RESTORE
	 
	echo "	if ($M_VAR) {">>$CODE_RESTORE
	echo "		$OBJ_VAR->$M_NAME = $M_VAR;">>$CODE_RESTORE
	echo "		MN_SET($M_VAR, $OBJ_VAR);">>$CODE_RESTORE
	echo "		$M_TYPE_NAME"_init"($M_VAR);">>$CODE_RESTORE
	echo "	}">>$CODE_RESTORE
	
	PROFILE_END $PROFILE>>$CODE_RESTORE       	


        #create free segment code file        
	 echo "">>$CODE_FREE
	 
	 PROFILE_START $PROFILE>>$CODE_FREE
	 
        if [ ! -z $BIT  ] && [ ! -z $BIT_NAME ]  && [ $BIT_NAME != "0" ]; then          
                echo "  if ($OBJ_VAR->who & $BIT_NAME)">>$CODE_FREE
                echo "  	$M_TYPE_NAME"_free"($OBJ_VAR->$M_NAME);">>$CODE_FREE
        else
                echo "  $M_TYPE_NAME"_free"($OBJ_VAR->$M_NAME);">>$CODE_FREE
        fi
        
        PROFILE_END $PROFILE>>$CODE_FREE  	
        

}

function OBJ_CPML_CLEAR(){
        local OBJ=$1
      
        local VAR_FILE=$OBJ"_var.c"
        local CODE_SAVE=$OBJ"_save.c"
        local CODE_RESTORE=$OBJ"_restore.c"
        local CODE_FREE=$OBJ"_free.c"
        local CODE_CREATE=$OBJ"_create.c"

	rm -f $VAR_FILE $CODE_SAVE $CODE_RESTORE $CODE_FREE $CODE_CREATE
}

function OBJ_CPML() {
	#create function
	local OBJ=$1
	local OUT_FILE=$2
	local OBJ_NAME=$OBJ
        local OBJ_VAR=$OBJ_NAME
        local OBJ_TYPE=$OBJ_NAME"_t"

        local VAR_FILE=$OBJ"_var.c"
        local CODE_SAVE=$OBJ"_save.c"
        local CODE_RESTORE=$OBJ"_restore.c"
        local CODE_FREE=$OBJ"_free.c"
        local CODE_CREATE=$OBJ"_create.c"
		
	local INIT_FUNC="__"$OBJ_NAME"_init"
	local FREE_FUNC="__"$OBJ_NAME"_free"
	local SUB_CREATE_FUNC="__"$OBJ_NAME"_create_subs"

	if [ -z $OUT_FILE ]; then
		OUT_FILE="obj_"$OBJ".c"
	fi
	
	echo "/*this file is auto generate, do not edit*/">$OUT_FILE
	echo "#include \"3g-mngr-include.h\"">>$OUT_FILE
	echo "">>$OUT_FILE
	echo "void $INIT_FUNC($OBJ_TYPE *$OBJ_VAR)">>$OUT_FILE
	echo "{">>$OUT_FILE
	cat $VAR_FILE>>$OUT_FILE
	echo "">>$OUT_FILE
	
	cat $CODE_SAVE>>$OUT_FILE
	echo "">>$OUT_FILE
	echo "	INIT_3G($OBJ_VAR, 0, $OBJ_TYPE, 0);">>$OUT_FILE
	echo "">>$OUT_FILE
	cat $CODE_RESTORE>>$OUT_FILE
	
	echo "}">>$OUT_FILE

	echo "">>$OUT_FILE
	echo "void $FREE_FUNC($OBJ_TYPE *$OBJ_VAR)">>$OUT_FILE
	echo "{">>$OUT_FILE
	cat $CODE_FREE>>$OUT_FILE
	echo "}">>$OUT_FILE


	echo "">>$OUT_FILE
	if [ $OBJ_NAME = "mngr" ]; then
		echo "void $SUB_CREATE_FUNC($OBJ_TYPE *$OBJ_VAR, unsigned int who)">>$OUT_FILE
	else
		echo "void $SUB_CREATE_FUNC($OBJ_TYPE *$OBJ_VAR)">>$OUT_FILE
	fi
	echo "{">>$OUT_FILE
	cat $CODE_CREATE>>$OUT_FILE
	echo "}">>$OUT_FILE
	echo "">>$OUT_FILE
}

function OBJ_CHECK_MEMBER(){
	FILE=$1

	TMP_FILE="member_check_tmp"
	
	sed -n '/OBJ_MEMBER/p' $FILE>$TMP_FILE
	
	i=1
	LINE=$(eval sed -n '"$i"p' $TMP_FILE)
	while [ ! -z $(echo $LINE|awk '{printf("%s", $1);}') ]; do
		PAR=$(echo $LINE|sed  -e 's/.*(//' -e 's/).*//' -e 's/,/ /g' -e 's/ \t//g')
		OBJ_MEMBER $PAR
		i=$((i+1))
		LINE=$(eval sed -n '"$i"p' $TMP_FILE)
	done

	rm -f $TMP_FILE

}

case $1 in
        member)
	OBJ_MEMBER $2 $3 $4 $5 $6
        ;;
        
	clear)
        OBJ_CPML_CLEAR $2
	;;
        
	cmpl)
        OBJ_CPML $2 $3
	OBJ_CPML_CLEAR $2
	;;

	file)
	echo "/*this file is auto generated,do not edit!*/">$BIT_DEF_FILE
	OBJ_CHECK_MEMBER $2	
	;;
        
	*)
	echo "support: member, clear, cmpl file"
	echo "not support $1"
        
	exit 3
esac

exit 0
