/*
 *
 * Nathan Gibson
 * myfind.c
 *
 * myfind emulates some of the functionality of the unix find command
 *
 * myfind requires a directory, and optionally takes a type (either file or 
 * directory) and a depth.
 *
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

/*
 *  checks to see if the given string is a file
 */
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

/*
 *  checks to see if the given string is a directory
 */
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

/*
 *   find takes a directory, a depth > 0, and a filter (f or d)
 *
 *   the directory is opened and traversed, directories and files
 *   are printed (if not filtered) and then the directories are
 *   recursively visited, this continues until either the maxdepth
 *   is met or there are no more directories to visit
 */
void find(char* directory, int maxdepth, char* type){
    
    //if maxdepth has reached 0 then recursing
    if (maxdepth != -1 && maxdepth == 0) {
        return;
    }
    
    DIR *fd;
    struct dirent *de;
    
    if ((fd = opendir(directory)) == NULL) {
        fprintf(stderr, "Unable to open '%s'\n",
                directory);
        exit(1);
    }
    
    while ((de = readdir(fd))) {
        //dont recurse on self or parent
        if (strcmp(de -> d_name, ".") == 0 || strcmp(de -> d_name, "..") == 0) {
            continue;
        }
        //build path from the passed in directory and the directory found in it
        char* path = (char*)malloc(sizeof(directory) + sizeof(de ->d_name) + 1);
        strcat(path, directory);
        strcat(path, "/");
        strcat(path, de -> d_name);
        
        if (is_file(path)) {
            //only print if the type file was supplied
            if (strcmp(type, "f") == 0 || strcmp(type, "b") == 0) {
                printf("%s\n", path);
            }
            
        }
        if(is_dir(path)){
            //only print if the type directory was supplied
            if (strcmp(type, "d") == 0 || strcmp(type, "b") == 0) {
                printf("%s\n", path);
            }
            //recursively visit directories in this folder
            find(path, maxdepth == -1 ? -1 : maxdepth -1, type);
        }
    }
    closedir(fd);
}

int main(int argc, char *argv[]){
    
    char* directory = " ";
    
    //default value for depth if it isnt supplied
    int maxdepth = -1;
    
    //default value for type
    char* type = "b";
    
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
    
    if (maxdepth != -1 && maxdepth < 0) {
        fprintf(stderr, "Incorrect depth\n");
        exit(1);
    }
    
    if (strcmp(directory, " ") == 0) {
        fprintf(stderr, "Usage: directory --type = <d or f> --maxdepth = <#>\n");
        exit(1);
    }
    
    if (maxdepth == 0) {
        printf("%s\n", directory);
    }
    
    find(directory, maxdepth, type);
}
