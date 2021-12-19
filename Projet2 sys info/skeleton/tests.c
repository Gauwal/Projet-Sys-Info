#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib_tar.h"

#define true 1

#define false 0


/**
 * You are free to use this file to write tests for your implementation
 */

void debug_dump(const uint8_t *bytes, size_t len) {
    for (int i = 0; i < len;) {
        printf("%04x:  ", (int) i);

        for (int j = 0; j < 16 && i + j < len; j++)  {
            printf("%02x ", bytes[i + j]);
        }
        printf("\t");
        for (int j = 0; j < 16 && i < len; j++, i++) {
            printf("%c ", bytes[i]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s tar_file\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1] , O_RDONLY);
    if (fd == -1) {
        perror("open(tar_file)");
        return -1;
    }

    int ret = check_archive(fd);
    printf("check_archive returned %d\n", ret);
    if(ret==20){
    	printf("SUCCES \n");
    }
    
    char* path = "dir/";
    ret = exists(fd, path);
    printf("true_exists returned %d\n", ret);
    if(ret==1){
    	printf("SUCCES \n");
    }
    path = "bobleponge";
    ret = exists(fd, path);
    printf("false_exists returned %d\n", ret);
    if(ret==0){
    	printf("SUCCES \n");
    }

    path = "dir/";
    ret = is_dir(fd, path);     
    printf("true_isdir returned %d\n", ret); 
    if(ret==1){
    	printf("SUCCES \n");
    }
    path = "non";
    ret = is_dir(fd, path);        
    printf("false_isdir returned %d\n", ret);   
    if(ret==0){
    	printf("SUCCES \n");
    }
    
    path = "dir/lib_tar.c";
    ret = is_file(fd, path);     
    printf("true_isdir returned %d\n", ret); 
    if(ret==1){
    	printf("SUCCES \n");
    }
    path = "non";
    ret = is_file(fd, path);        
    printf("false_isdir returned %d\n", ret);   
    if(ret==0){
    	printf("SUCCES \n");
    }
    
    path = "dir/symlink";
    ret = is_symlink(fd, path);     
    printf("true_symlink returned %d\n", ret); 
    if(ret==1){
    	printf("SUCCES \n");
    }
    path = "non";
    ret = is_symlink(fd, path);        
    printf("false_symlink returned %d\n", ret);   
    if(ret==0){
    	printf("SUCCES \n");
    }
    char **entries = malloc(sizeof(char*) * 40); 
    size_t *no_entries = malloc(sizeof(size_t));
    for(int i =0; i<40;i++){ 
    	entries[i]=malloc(100*sizeof(char));
    }
    *no_entries = 40;
    path = "dir/";
    
    ret = list(fd, path, entries, no_entries); 
    
    for(int i =0; i<*no_entries;i++){ 
    	printf("%s\n",entries[i]);
    }       
    printf("list returned %d\n", ret); 
     
    if(ret==8){
    	printf("SUCCES \n");
    }
    
    path = "dir/dir2/Symlink2";
    uint8_t *dest = malloc(35*sizeof(uint8_t));
    size_t *len = malloc(sizeof(size_t));
    *len=35;
    ret = read_file(fd, path,1, dest, len);   
         
    
    for (int i = 0; i <*len; i++){
    	printf("%c", dest[i]);
    }
    printf("\n");
    
    printf("read_file returned %d\n", ret);
    
    if(ret==0 ){
    	printf("SUCCES \n");
    }
    
    
    
    return 0;
}
