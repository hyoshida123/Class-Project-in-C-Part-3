//
//  main.c
//  HYoshidaHtmlSrcList
//
//  Created by Hideaki Yoshida on 2017/02/26.
//  Copyright © 2017年 Hideaki Yoshida. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "ctype.h"

void getCount(char* html);
void getTags(char* html);
void getUrls(char* html);
void getFrequencies(char* html);

int main(int argc, const char * argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);
    
    char url[BUFSIZ + 1];
    char buffer[BUFSIZ + 1];
    char htmlDoc[256 * 1024];
    char curl[40] = "curl -s ";
    char input[4];
    int flag = 1;
    FILE* pFile = NULL;
    
    printf("URL:\n");
    // Get the url from a user.
    fgets(url, BUFSIZ, stdin);
    url[strlen(url) - 1] = '\0';
    // Append url to execute "curl -s www.sbcc.edu" and popen HTML.
    pFile = popen(strcat(curl, url), "r");
    // Read every line on HTML and append them on array htmlDoc.
    while(!feof(pFile)){
        fgets(buffer, BUFSIZ, pFile);
        strcat(htmlDoc, buffer);
    }
    // Get commands from a user until the user hits q.
    do {
        printf("Ready\n");
        fgets(input, 8, stdin);
        switch (input[0]) {
            case 'c':
                getCount(htmlDoc);
                break;
            case 't':
                getTags(htmlDoc);
                break;
            case 'u':
                getUrls(htmlDoc);
                break;
            case 'f':
                getFrequencies(htmlDoc);
                break;
            case 'q':
                printf("Complete\n");
                flag = 0;
                break;
        }
    } while (flag != 0);
    
    pclose(pFile);
    
    return 0;
}

void getCount(char* html){
    char* pSrc = html;
    int count = 0;
    // Search for src in HTML where html points
    while ((pSrc = (strstr(pSrc, "src=\""))) != NULL) {
        // Count if there's space before src
        if (isspace(*(pSrc - 1))) {
            count++;
        }
        pSrc++;
    }
    printf("%d\n", count);
}

void getTags(char* html){
    char* pSrc = html;
    char* ptr1 = NULL;
    while ((pSrc = (strstr(pSrc, "src=\""))) != NULL) {
        ptr1 = pSrc;
        if (isspace(*(ptr1 - 1))) {
            // When src is valid, make ptr1 point to <.
            while (*(ptr1 - 1) != '<') {
                ptr1--;
            }
            // Print each character until ptr1 points to a space.
            while (!isspace(*ptr1)) {
                printf("%c", *ptr1);
                ptr1++;
            }
            printf("\n");
        }
        pSrc++;
    }
}

void getUrls(char* html){
    char* pSrc = html;
    char* pQ1 = NULL;
    char* pQ2 = NULL;
    char buffer[BUFSIZ + 1];
    int numChar;
    while ((pSrc = (strstr(pSrc, "src=\""))) != NULL) {
        numChar = 0;
        if (isspace(*(pSrc - 1))) {
            pQ1 = pSrc;
            // pQ1 keeps moving forwards until it points to '\"'.
            while(*pQ1 != '\"'){
                pQ1++;
            }
            pQ2 = pQ1 + 1;
            while(*pQ2 != '\"'){
                pQ2++;
                numChar++;
            }
            strncpy(buffer, pQ1 + 1, numChar);
            // Add '\n' and '\0' at the end ot url.
            buffer[numChar] = '\n';
            buffer[numChar + 1] = '\0';
            printf(buffer);
        }
        pSrc++;
    }
}

void getFrequencies(char* html){
    char* pSrc = html;
    char* ptr1 = NULL;
    char* buffer[BUFSIZ+1];
    int i = 0;
    int j;
    int numTags = 0;
    
    while ((pSrc = (strstr(pSrc, "src=\""))) != NULL) {
        if (isspace(*(pSrc-1))) {
            ptr1 = pSrc;
            // When src is valid, make ptr1 point to <.
            while (*(ptr1-1) != '<') {
                ptr1--;
            }
            buffer[i] = ptr1;
        }
        pSrc++;
        i++;
    }
    for(j=0;j < i;j++){
        if(strcmp(buffer[0],buffer[j]) == 0){
            numTags++;
            buffer[j] = NULL;
        }
    }
    printf("%s\t%d", *buffer, numTags);

}
