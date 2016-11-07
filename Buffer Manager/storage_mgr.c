#include<stdio.h>

#include<stdlib.h>
#include"storage_mgr.h"
#include"dberror.h"

FILE *file;
int pagetoberead;


void initStorageManager()
{
	printf("Storage Manager Initialized\n");
}

RC createPageFile(char *fname)
{	
	int i;
	char choice;
	file=fopen(fname,"r");
	if(file!=NULL)
	{
		printf("File already exists. Would you like to overwrite it?(y/n)");
		scanf("%c",&choice);
		fclose(file);
	}
	if(file==NULL || choice=='y')
	{
		file=fopen(fname,"w");
		char *ptr=(char *)calloc(PAGE_SIZE,sizeof(char));
		fwrite(ptr,PAGE_SIZE,sizeof(char),file);
		fseek(file,0,SEEK_END);
		free(ptr);
		/*fseek(file , 0 , SEEK_SET);//seeking to the beginning of the file
//filling the page of page size 4096 bytes
for(i = 0; i < PAGE_SIZE; i++)
{
fwrite("\0",1, 1,file);
fseek(file,0,SEEK_END);
}*/
	
		fclose(file); 
		return RC_OK;
	}
	else
	{
		RC_message="File Already Present";
		return RC_FILE_ALREADY_PRESENT;
		
	}
	
} 

RC openPageFile(char *fname,SM_FileHandle *fHandle)
{
	fHandle->mgmtInfo=fopen(fname,"r");
	int fPageCount,fSize;
	
	
		fHandle->mgmtInfo=fopen(fname,"r");
		fHandle->fileName=fname;
		fHandle->curPagePos=ftell(fHandle->mgmtInfo)/PAGE_SIZE;
		fseek(fHandle->mgmtInfo,0,SEEK_END);
		fSize=ftell(fHandle->mgmtInfo);
		fPageCount=fSize/PAGE_SIZE;
		fHandle->totalNumPages=fPageCount;
		fclose(fHandle->mgmtInfo);
		return RC_OK;
		
	
}

RC destroyPageFile(char *fileName)
{
	if(fopen(fileName,"r") != NULL)
	{
		remove(fileName);
		return RC_OK;
	}	
	else
	{
		return RC_FILE_NOT_FOUND;
	}
} 

RC closePageFile(SM_FileHandle *fHandle)
{
	if(fHandle != NULL)
	{
		if(fopen(fHandle->fileName,"r") != NULL)
		{
			if(fclose(fHandle->mgmtInfo) == 0)
			{
				return RC_OK;
			}
			else
			{
				return RC_FILE_NOT_FOUND;
			}
		}
		else return RC_FILE_NOT_FOUND;

	}else return RC_FILE_HANDLE_NOT_INIT;
}




RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle !=NULL)
	{
		fHandle->mgmtInfo=fopen(fHandle->fileName,"r");
		if(fHandle->mgmtInfo != NULL)
		{
			if((pageNum)>-1 && (pageNum+1)<=fHandle->totalNumPages)
			{
				if(memPage !=NULL)
				{
					fseek(fHandle->mgmtInfo,(((pageNum)*PAGE_SIZE)),SEEK_SET);
					//char *ptr=(char *)malloc(PAGE_SIZE);
					fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);
					/*if(fread(memPage,1,PAGE_SIZE,fHandle->mgmtInfo)==PAGE_SIZE)
					{
						fHandle->curPagePos=pageNum;
						return RC_OK;
					}
					else
					{
						return RC_FILE_READ_ERROR;
					}	*/
					fHandle->curPagePos=pageNum;
					fclose(fHandle->mgmtInfo);
					return RC_OK;
				}
			}
			else
			{
				RC_message="Page to be read doesn't exist in file";
				return RC_READ_NON_EXISTING_PAGE;
			}
		}
		else
		{
			RC_message="File Not Found";
			return RC_FILE_NOT_FOUND;
		}
	}
	else
	{
		//RC_message="desired file related data is not initialized";
		return RC_FILE_HANDLE_NOT_INIT;
	}	
}

int getBlockPos (SM_FileHandle *fHandle)
{
	if(fHandle != NULL)
	{
		if((fopen(fHandle->fileName,"r")) != NULL)
		{
			printf("%d ",fHandle->curPagePos);
			return fHandle->curPagePos;
		}
		else
		{
			//RC_message="File Not Found"; 
			return RC_FILE_NOT_FOUND;
		}
	}
	else
	{
		//RC_message="desired file related data is not initialized";
		return RC_FILE_HANDLE_NOT_INIT;
	}
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=0;
	readBlock(pagetoberead,fHandle, memPage);
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=getBlockPos(fHandle)-1;
	readBlock (pagetoberead,fHandle, memPage);
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=getBlockPos(fHandle);
	readBlock (pagetoberead,fHandle, memPage);
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=getBlockPos(fHandle)+1;
	readBlock (pagetoberead,fHandle, memPage);
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=(fHandle->totalNumPages)-1;
	readBlock (pagetoberead,fHandle, memPage);
}


RC appendEmptyBlock (SM_FileHandle *fHandle)
{
	fHandle->mgmtInfo=fopen(fHandle->fileName,"a");
	if ((fHandle != NULL) && fHandle->mgmtInfo != NULL)
	{
		//fHandle->mgmtInfo = fopen(fHandle->fileName,"a");
		fHandle->totalNumPages+=1;
		fseek(fHandle->mgmtInfo, (((fHandle->totalNumPages)*PAGE_SIZE)), SEEK_END);
		char *stream = (char *)calloc(PAGE_SIZE, sizeof(char));
		fwrite(stream, PAGE_SIZE, sizeof(char), fHandle->mgmtInfo);
		free(stream);
		fclose(fHandle->mgmtInfo);
		return RC_OK;
	}
	else
	{
		if (fHandle == NULL)   
			return RC_FILE_HANDLE_NOT_INIT;
		else
			return RC_FILE_NOT_FOUND;
	}
}



RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
	fHandle->mgmtInfo=fopen(fHandle->fileName,"r");
	if ((fHandle != NULL) && (fHandle->mgmtInfo != NULL))
	{
		if (fHandle->totalNumPages < numberOfPages)
		{
			while (fHandle->totalNumPages != numberOfPages) 
			{
				fclose(fHandle->mgmtInfo);
				appendEmptyBlock(fHandle);
			}
		}
		else
		{
			return RC_NUMBER_OF_PAGES_EXISTS;
		}
	}
	else
	{
		if (fHandle == NULL)   
			return RC_FILE_HANDLE_NOT_INIT;
		else
			return RC_FILE_NOT_FOUND;
	}
		
}


RC UpdateFileStats(SM_FileHandle *fHandle)
{

    //file = fopen(fHandle->fileName,"r");
    fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE);
    fseek(fHandle->mgmtInfo, (fHandle->curPagePos)*PAGE_SIZE , SEEK_SET);
    //fHandle->mgmtInfo = file;
	fclose(fHandle->mgmtInfo);
    return RC_OK;
}



RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if(fHandle != NULL)
    {
	fHandle->mgmtInfo=fopen(fHandle->fileName,"r+");
        if((fHandle->mgmtInfo) != NULL)
        {
                if((pageNum <= (fHandle->totalNumPages)))
                {
                    if(memPage != NULL)
                    {
                            //file = fopen(fHandle->fileName,"r+");
                            fseek(fHandle->mgmtInfo, pageNum*PAGE_SIZE , SEEK_SET);
                            if(fwrite(memPage,PAGE_SIZE,1,fHandle->mgmtInfo) != 1)
                            { 
				return RC_WRITE_FAILED;
			    }
                            else
			    {
				UpdateFileStats(fHandle);
			    }
                    }

                }
                else return RC_WRITE_OUT_OF_BOUND_INDEX;
        }
        else return RC_FILE_NOT_FOUND;
    }
    else return RC_FILE_HANDLE_NOT_INIT;
}


RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if(fHandle != NULL)
    {
        if((fopen(fHandle->fileName,"r")) != NULL)
        {
                if((fHandle->curPagePos <= (fHandle->totalNumPages)))
                {
                    if(memPage != NULL)
                    {
                        writeBlock(fHandle->curPagePos, fHandle, memPage);
                    }

                }
                else return RC_WRITE_OUT_OF_BOUND_INDEX;
        }
        else return RC_FILE_NOT_FOUND;
    }
    else return RC_FILE_HANDLE_NOT_INIT;
}
/*
void main()
{
	
	int a,i;
	SM_FileHandle fh;
	ph = (SM_PageHandle) malloc(PAGE_SIZE);
	initStorageManager();
	createPageFile("Hello.txt");
	openPageFile("Hello.txt", &fh);
	ensureCapacity(11,&fh);
	//destroyPageFile("Hello.txt");	
	//closePageFile(&fh);
	//readBlock(1,&fh,ph);
	//getBlockPos(&fh);
	/*readFirstBlock(&fh, ph);
	
	readCurrentBlock(&fh, ph);*/
	//readNextBlock(&fh, ph);
	//readLastBlock(&fh, ph);
	//appendEmptyBlock(&fh);
	//readLastBlock(&fh, ph);
	//readPreviousBlock(&fh, ph);
	//readPreviousBlock(&fh, ph);
	
	/*readNextBlock(&fh,ph);
	for (i=0; i < PAGE_SIZE; i++)
	{ ph[i] = 'a';
		printf("%c",ph[i]);	 
	}*/
	
	//writeCurrentBlock(&fh,ph);
	//readCurrentBlock(&fh,ph);
	/*appendEmptyBlock(&fh);
	readNextBlock(&fh, ph);
	readNextBlock(&fh, ph);
	appendEmptyBlock(&fh);
	/*for (i=0; i < PAGE_SIZE; i++)
	ph[i] = (i % 10) + '0';
	a = writeBlock(0,&fh,ph);
	printf("RC: %d write block\n", a);
	for (i=0; i < PAGE_SIZE; i++)
	ph[i] = '0';
	a = writeCurrentBlock(&fh,ph);
	printf("RC: %d write current block\n", a);
	a = readNextBlock(&fh, ph);
	printf("RC: %d read Next block\n", a);
	a = appendEmptyBlock(&fh);
	printf("RC: %d Append Empty Block!!\n", a);
	a = readNextBlock(&fh, ph);
	printf("RC: %d read Next block\n", a);
	//a = ensureCapacity(10,&fh);
	//printf("RC: %d Ensure Capacity!!\n", a);
	a = readNextBlock(&fh, ph);
	printf("RC: %d read Next block\n", a);
}
	*/

	
