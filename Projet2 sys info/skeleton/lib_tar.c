#include "lib_tar.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define bool unsigned int

#define true 1

#define false 0


/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */

int min(double a,double b){
	if(a<b){
		return a;
	}
	return b;
}
int power(long int base, long int exp){
	long int result=1;
	for (; exp>0; exp--)
	{
		result *= base;
	}
	return result;
}

int check_archive(int tar_fd) {
    lseek(tar_fd,0,SEEK_SET);//reset to file start 
    tar_header_t* header= malloc(sizeof(tar_header_t));
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(tar_fd,vrai_list,1024*sizeof(char));
    lseek(tar_fd,0,SEEK_SET);
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    long int octalSize;
    long int decimalSize;
    int i;
    while(strncmp(test_list,vrai_list,1024*sizeof(char))){ 
    	
    	
    	read(tar_fd,header,sizeof(tar_header_t));
    	lseek(tar_fd,(off_t)-sizeof(tar_header_t),SEEK_CUR);
    	
        if(strncmp(header->magic,TMAGIC,6)){
		free(header);
		free(test_list);
		free(vrai_list);
        	return -1;
        	}
       
    	if(strncmp(header->version,TVERSION,2)){
    		free(header);
		free(test_list);
		free(vrai_list);
    	
        	return -2;
        	}
    	uint8_t *ptr = (uint8_t*) header;
    	//int size = (int)sizeof(tar_header_t); //useless
        int offset=0;
    	uint32_t sum = 0;
    	while(offset!=(int)sizeof(tar_header_t)){
	    if(offset>=148 && offset<156){ ///checkintest                                                                                       
                sum+= (int) '0';
            }
            sum += *(ptr+offset);
            offset++;
		}
    	if(*((uint32_t*)(header->chksum))==sum){
        	return -3;
        }

        if (header->typeflag == DIRTYPE){
        
        	lseek(tar_fd,(off_t)sizeof(tar_header_t),SEEK_CUR);
        }
        else{
        	octalSize=atoi(header->size);
        	decimalSize = 0;
        	i =0;


    		while (octalSize != 0)

    		{

        		decimalSize =  decimalSize +(octalSize % 10)* power(8, i++);

       		 octalSize = octalSize / 10;

   		}


        	lseek(tar_fd,((decimalSize/512 * 512) + (decimalSize % 512 !=0)*512) +sizeof(tar_header_t),SEEK_CUR);
        	
        }
        
        read(tar_fd,vrai_list,1024*sizeof(char));
        lseek(tar_fd,(off_t)-1024*sizeof(char),SEEK_CUR); 
        
    }
    free(header);
    free(test_list);
    free(vrai_list);
    return 0;
}

/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd, char *path) {
    lseek(tar_fd,0,SEEK_SET);//reset to file start
    tar_header_t* header= malloc(sizeof(tar_header_t));
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(tar_fd,vrai_list,sizeof(vrai_list));
    lseek(tar_fd,0,SEEK_SET);
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    long int octalSize;
    long int decimalSize;
    int i;
    while(strncmp(test_list,vrai_list,1024*sizeof(char))){
        read(tar_fd,header,sizeof(tar_header_t));
        lseek(tar_fd,(off_t)-sizeof(tar_header_t),SEEK_CUR);
        if (strcmp(header->name,path)==0){
            
                
            free(test_list);
            free(vrai_list);
            return 1;
        }
        
	if (header->typeflag == DIRTYPE){
        
        	lseek(tar_fd,(off_t)sizeof(tar_header_t),SEEK_CUR);
        }
        else{
        	octalSize=atoi(header->size);
        	decimalSize = 0;
        	i =0;


    		while (octalSize != 0)

    		{

        		decimalSize =  decimalSize +(octalSize % 10)* power(8, i++);

       		 octalSize = octalSize / 10;

   		}


        	lseek(tar_fd,((decimalSize/512 * 512) + (decimalSize % 512 !=0)*512) +sizeof(tar_header_t),SEEK_CUR);
        	
        }
        
        read(tar_fd,vrai_list,1024*sizeof(char));
        lseek(tar_fd,(off_t)-1024*sizeof(char),SEEK_CUR);
    }    
    free(header);
    free(test_list);
    free(vrai_list);
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd, char *path) {
    lseek(tar_fd,0,SEEK_SET);//reset to file start
    if(exists(tar_fd, path)){
        tar_header_t* header= malloc(sizeof(tar_header_t));
        read(tar_fd,header,sizeof(tar_header_t));
        if(header->typeflag==DIRTYPE){
            lseek(tar_fd,0,SEEK_SET);
            free(header);
            return 1;
        }
    }
    lseek(tar_fd,0,SEEK_SET);
    
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {
    lseek(tar_fd,0,SEEK_SET);//reset to file start

    if(exists(tar_fd, path)){
        tar_header_t* header= malloc(sizeof(tar_header_t));
        read(tar_fd,header,sizeof(tar_header_t));
        if((header->typeflag==REGTYPE)||(header->typeflag==AREGTYPE)){
            lseek(tar_fd,0,SEEK_SET);
            free(header);
            return 1;
        }
    }
    lseek(tar_fd,0,SEEK_SET);
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {
    lseek(tar_fd,0,SEEK_SET);//reset to file start
    
    if(exists(tar_fd, path)){
        tar_header_t* header= malloc(sizeof(tar_header_t));
        read(tar_fd,header,sizeof(tar_header_t));
        if((header->typeflag==SYMTYPE)||(header->typeflag==LNKTYPE)){
            lseek(tar_fd,0,SEEK_SET);
            free(header);
            return 1;
        }
    }
    lseek(tar_fd,0,SEEK_SET);
    return 0;
}
/**
 * Lists the entries at a given path in the archive.
 * list() does not recurse into the directories listed at the given path.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         any other value otherwise.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {
    
    lseek(tar_fd,0,SEEK_SET);//reset to file start
    
    tar_header_t* header= malloc(sizeof(tar_header_t));
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(tar_fd,vrai_list,sizeof(vrai_list));
    lseek(tar_fd,0,SEEK_SET);
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    long int octalSize;
    long int decimalSize;
    int i;
   
    char* chked_path;
    char* chker=malloc(strlen(path));
    bool equal;
    long int offset=0;
    
    bool valid;
    
    int count = 0;

    if (is_dir(tar_fd,path)){
    	printf("%s \n",path);
    	valid=true;
    	count++;
    }

    
    while(strncmp(test_list,vrai_list,1024*sizeof(char))&&valid){
    	
    	read(tar_fd,header,sizeof(tar_header_t) );
    	lseek(tar_fd,-sizeof(tar_header_t),SEEK_CUR);

    	
    	
    	if (strlen(path)<strlen(header->name)){
    		
    		chked_path=malloc(strlen(header->name));
    		
    		equal= true;

    		strcpy(chked_path,header->name);
    		strcpy(chker,path);
    		
    		while  (*chker != '\0' && equal){
    			
    			if (*chked_path == *chker){
    				
    				chked_path++;
    				chker++;
    			}
    			
    			else {
    				
    				equal=false;
    			}
    			
    		}
    		if (equal){
    			if (strchr(chked_path,'/')==NULL){
    			
    	    			if (is_symlink(tar_fd,header->name)){
    	    				i=0;
    					while (strchr(header->name + i,'/')!=NULL){
    				
    						i++;
    					}
    					char* dirPath= malloc(i*sizeof(char)+strlen(header->linkname));
    					strncpy(dirPath,header->name,i);
    					dirPath[i]='\0';
    					strcat(dirPath,header->linkname);
    	    				printf("%s \n",dirPath);
    	    			}
    	    			else{
    					printf("%s \n",header->name);
    				}
    				count++;
    			
    	    		}
    	    		else{
    	    			if(*(strchr(chked_path,'/')+1)=='\0'){
    	    	    			if(is_dir(tar_fd,header->name)){
    	    	    				

    			
    						printf("%s \n",header->name);
    						count++;
    						
    						
    						
    				
    						lseek(tar_fd,0,SEEK_SET);
    	    				}
    	    	
    	    			}
    	    		}
    	    	}
    	    	
    	}
    	
	if (header->typeflag == DIRTYPE){
		offset+=sizeof(tar_header_t);
        
        	lseek(tar_fd,offset,SEEK_SET);
        }
        else{
        	octalSize=atoi(header->size);
        	decimalSize = 0;
        	i =0;


    		while (octalSize != 0)

    		{

        		decimalSize =  decimalSize +(octalSize % 10)* power(8, i++);

       		 octalSize = octalSize / 10;

   		}
   		
   		offset+= ((decimalSize/512 * 512) + (decimalSize % 512 !=0)*512)+sizeof(tar_header_t);
        	lseek(tar_fd,offset ,SEEK_SET);
        	
        }
        
        read(tar_fd,vrai_list,1024*sizeof(char));
        lseek(tar_fd,(off_t)-1024*sizeof(char),SEEK_CUR);
    }
    
    
    if ( valid){
    	*no_entries=count;
    	free(header);
    	free(test_list);
    	free(vrai_list);
    	
    	
    	return count;
    }
    free(header);
    free(test_list);
    free(vrai_list);
    
    return 0;
}

/**
 * Reads a file at a given path in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive to read from.  If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param offset An offset in the file from which to start reading from, zero indicates the start of the file.
 * @param dest A destination buffer to read the given file into.
 * @param len An in-out argument.
 *            The caller set it to the size of dest.
 *            The callee set it to the number of bytes written to dest.
 *
 * @return -1 if no entry at the given path exists in the archive or the entry is not a file,
 *         -2 if the offset is outside the file total length,
 *         zero if the file was read in its entirety into the destination buffer,
 *         a positive value if the file was partially read, representing the remaining bytes left to be read to reach
 *         the end of the file.
 *
 */
ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) {
    lseek(tar_fd,0,SEEK_SET);//reset to file start
    
    tar_header_t* header= malloc(sizeof(tar_header_t));
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(tar_fd,vrai_list,sizeof(vrai_list));
    
    long int octalSize;
    long int decimalSize;
    int i;
    
    int off = 0;
    
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    //int size=0; //useless
    
    
    lseek(tar_fd,0,SEEK_SET);
    
    while(strncmp(test_list,vrai_list,1024*sizeof(char))){
    
    	read(tar_fd,header,sizeof(tar_header_t) );
    	lseek(tar_fd,-sizeof(tar_header_t),SEEK_CUR);
    	
    	
    	
    	if (!strcmp(header->name,path)){
    		
    		if(is_symlink(tar_fd,header->name)){
    			i=0;
    			while (strchr(header->name + i,'/')!=NULL){
    				
    				i++;
    			}
    			char* dirPath= malloc(i*sizeof(char)+strlen(header->linkname));
    			strncpy(dirPath,header->name,i);
    			dirPath[i]='\0';
    			strcat(dirPath,header->linkname);
    			
    			return read_file(tar_fd, dirPath, offset, dest, len);
    		}
    		
        	octalSize=atoi(header->size);
        	decimalSize = 0;
        	i =0;


    		while (octalSize != 0)

    		{

        		decimalSize =  decimalSize +(octalSize % 10)* power(8, i++);

       		 octalSize = octalSize / 10;

   		}
    	
    		if ((int)offset>=decimalSize){
    		    	free(header);
    		    	free(test_list);
    		    	free(vrai_list);
    			return -2;
    		}
    		else{
    			lseek(tar_fd,off+offset+sizeof(tar_header_t),SEEK_SET);
    			
    			*len = min((double) *len,decimalSize-(int)offset);//amount we read
    			
    			read(tar_fd,dest,*len);
    			
    			free(header);
    		    	free(test_list);
    		    	free(vrai_list);
    			return 0;
    		}
    	
    		
    	
    	}
    	
    	
	if (header->typeflag == DIRTYPE){
        	off += sizeof(tar_header_t);
        	lseek(tar_fd,off,SEEK_SET);
        }
        else{
        	octalSize=atoi(header->size);
        	decimalSize = 0;
        	i =0;


    		while (octalSize != 0)

    		{

        		decimalSize =  decimalSize +(octalSize % 10)* power(8, i++);

       		octalSize = octalSize / 10;

   		}

		off += ((decimalSize/512 * 512) + (decimalSize % 512 !=0)*512) +sizeof(tar_header_t);
        	lseek(tar_fd,off,SEEK_SET);
        	
        }
        
        read(tar_fd,vrai_list,1024*sizeof(char));
        lseek(tar_fd,(off_t)-1024*sizeof(char),SEEK_CUR);
    }    
    free(header);
    free(test_list);
    free(vrai_list);
    return -1;
}
