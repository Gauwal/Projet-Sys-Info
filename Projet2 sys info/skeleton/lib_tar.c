#include "lib_tar.h"

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
int check_archive(int tar_fd) {
    lseek(tar_fd,0,SEEK_SET);//reset to file start 
    tar_header_t* header;
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(vrai_list,tar_fd,sizeof(vrai_list));
    int off = 0;
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    while(strncomp(test_list,vrai_list,sizeof(vrai_list))){    
    	read(header,tar_fd,sizeof(tar_header_t));
        if(strncmp(header->magic,TMAGIC,6)){
        	return -1;
        	}
    	if(strncmp(header->version,TVERSION,6)){
        	return -2;
        	}
    	uint8_t *ptr = (uint8_t*) header;
    	int size = (int)sizeof(tar_header_t);
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
        for (int i = 0;  i<12; i++){
    		off+=((int) * header->size+(11-i))<<((11-i)*8);
    	}
        lseek(tar_fd,(off_t)off,SEEK_SET);
        read(vrai_list,tar_fd,sizeof(vrai_list));
    }
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
    tar_header_t* header;
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(vrai_list,tar_fd,sizeof(vrai_list));
    int off = 0;
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    while(strncomp(test_list,vrai_list,sizeof(vrai_list))){
        read(header,tar_fd,sizeof(tar_header_t));
        if (strncomp(header->name,path)){
            
            return 1;
        }
        
        for (int i = 0;  i<12; i++){
    		off+=((int) * header->size+(11-i))<<((11-i)*8);
    	}
        lseek(tar_fd,(off_t)off,SEEK_SET);
        read(vrai_list,tar_fd,sizeof(vrai_list));
    }    
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
        if(tar_fd->typeflag==DIRTYPE){
            return 1;
        }
    }
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
        if((tar_fd->typeflag==REGTYPE)||(tar_fd->typeflag==AREGTYPE)){
            return 1;
        }
    }
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
        if((tar_fd->typeflag==SYMTYPE)||(tar_fd->typeflag==LINKTYPE)){
            return 1;
        }
    }
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
    
    tar_header_t* header;
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(vrai_list,tar_fd,sizeof(vrai_list));
    int off = 0;
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    while(strncomp(test_list,vrai_list,sizeof(vrai_list))){
    
    	read(header,tar_fd,sizeof(tar_header_t) );
    	
    	/*if (is_symlink(tar_fd,header->path){
    		char* hpath = header->linkname;
    	}
    	else{
    		char* hpath = header->path ;
    	}*/
    	
    	if (strlen(path)<strlen(header->path)){
    	
    		bool equal = true;
    		char* chked_path = header->path;
    		char* chker = path;
    		
    		while  (*chker != '0' && equal){
    			
    			if (*chked_path == *chker){
    				
    				chked_path++;
    				chker++;
    			}
    			
    			else {
    				
    				equal=false;
    			}
    		}
    		if (equal && (strchr(chked_path,'/')==NULL )){
    			
    			if is_dir(tar_fd,header->path){
    				printf("└── %s \n",chked_path);
    				list(tar_fd,header->path,entries,no_entries);
    	    		}
    	    		
    	    		else{
    				printf("└── %s \n",chked_path);
    			}
    	    	}
    
    	}
    	
    	for (int i = 0;  i<12; i++){//read 12 byte int
    		off+=((int) * header->size +(11-i))<<((11-i)*8);//might have problem with int size (<12 bytes)
    	}
    	
    	lseek(tar_fd,(off_t)off,SEEK_SET);
    	read(vrai_list,tar_fd,sizeof(vrai_list));
    }
    //set no_entries
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
    
    tar_header_t* header;
    char* test_list = malloc(1024*sizeof(char));
    char* vrai_list = malloc(1024*sizeof(char));
    read(vrai_list,tar_fd,sizeof(vrai_list));
    int off = 0;
    for(int i=0;i<1024;i++){
        test_list[i]=(char)0;
    }
    int size=0;
    while(strncomp(test_list,vrai_list,sizeof(vrai_list))){
    
    	read(header,tar_fd,sizeof(tar_header_t) );
    	
    	if (strncomp(header->name,path) == 0){
    		
    		for (int i = 0;  i<12; i++){//read 12 byte int
    			size+=((int) * header->size+(11-i))<<((11-i)*8);
    		}
    	
    		if ((int)offset>=size){
    			return -2;
    		}
    		else{
    			lseek(tar_fd,(off_t)offset,SEEK_SET);
    			read(dest,tar_fd,fmin((double) *len,size-(int)offset));
    			*len = fmin((double) *len,size-(int)offset);
    			return 0;
    		}
    	
    		
    	
    	}
    	
    	
    	
    	
    	for (int i = 0;  i<12; i++){//read 12 byte int
    		off+=((int) * header->size+(11-i))<<((11-i)*8);
    	}
    	
    	lseek(tar_fd,(off_t)off,SEEK_SET);
    	read(vrai_list,tar_fd,sizeof(vrai_list));
    }
    
    return -1;
}
