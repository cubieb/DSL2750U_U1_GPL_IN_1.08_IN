// test1.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printHelp()
{
  printf("parameter is error!please use like this:\r\n");
  printf("formatcsv csvfile column_num");
}

#define MAX_BUF_SIZE 1024
int main(int argc,char **argv)
{
	char buf[MAX_BUF_SIZE] = {0};
        if(argc != 3)
        {
          printHelp();
          return 1;
        }

       int iColumn = atoi(argv[2]);
       char *pFileName = argv[1];
       FILE *pFile = fopen(pFileName,"r");

       char cNode = 0;
       int iCurColumn = 0;
       int iCurNum = 0;
       while((cNode=fgetc(pFile))!=EOF)
       {
		if(cNode == ',')
		{
			iCurColumn++;
			buf[iCurNum++] = cNode;
		}
		else if((cNode != 0x0a) && (cNode != 0x0d))
		{
			buf[iCurNum++] = cNode;
		}
		else
		{
			;
		}

		if(iCurColumn == iColumn)
		{
			printf("%s\r\n",buf);
			memset(buf,0,sizeof(buf));
			iCurColumn = 0;
			iCurNum = 0;
		}
       }

	return 0;

}


