//
//  myfind.c
//  myfind
//
//  Created by Nathan Gibson on 5/1/15.
//  Copyright (c) 2015 Nathan Gibson. All rights reserved.
//
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int is_file(char* file){
    struct stat fd_stat;
    if (stat(file, &fd_stat)) {
        fprintf(stderr, "Unable to stat '%s'\n", file);
        exit(1);
    }
    if (S_ISREG (fd_stat.st_mode)) {
        return 1;
    }
    return 0;
}

int is_dir(char* file){
    struct stat fd_stat;
    if (stat(file, &fd_stat)) {
        fprintf(stderr, "Unable to stat '%s'\n", file);
        exit(1);
    }
    if (S_ISDIR(fd_stat.st_mode)) {
        return 1;
    }
    return 0;
}

void find(char* directory, int maxdepth, char* type){
    
    if (maxdepth != -1 && maxdepth == 0) {
        printf("%s\n", directory);
        return;
    }
    
    
    
    DIR *fd;
    struct dirent *de;
    
    if ((fd = opendir(directory)) == NULL) {
        fprintf(stderr, "Unable to open '%s'\n",
                directory);
        exit(3);
    }
        
    
    while ((de = readdir(fd))) {
        
        if (strcmp(de -> d_name, ".") == 0 || strcmp(de -> d_name, "..") == 0) {
            continue;
        }
        
        char* path = (char*)malloc(sizeof(directory) + sizeof(de ->d_name) + 1);
        strcat(path, directory);
        strcat(path, "/");
        strcat(path, de -> d_name);
        
        if (is_file(path)) {
            printf("%s\n", path);
        }
        if(is_dir(path)){
            printf("%s\n", path);
            find(path, maxdepth == -1 ? -1 : maxdepth -1, type);
        }
        
    }
    closedir(fd);
}

int main(int argc, char *argv[]){
    
    char* directory;
    int maxdepth = -1;
    char* type;
    
    if (argc > 1) {
        directory = argv[1];
    }else{
        fprintf(stderr, "No directory supplied\n");
    }
    
    for (int i = 2; i < argc; i++) {
        if (strcmp("-type", argv[i]) == 0) {
            type = argv[i+1];
        }else if (strcmp("-maxdepth", argv[i]) == 0){
            maxdepth = atoi(argv[i + 1]);
        }
    }
    
    find(directory, maxdepth, type);
}
