/*************************************************************************
  > File Name: jsonshow.c
  > Author: ayong www.openwrtdl.com
  > Mail: jk110333@126.com
  > Created Time: 2018年08月13日 星期一 15时11分06秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>
#include "cJSON.h"

static int better_print = 1; //better for shell script
#define VERSION "V0.1.0"

/*
cmd:cut root string
 */
int parase_object(cJSON*root,char *cmd)
{
    char *ptr=NULL;
    cJSON *tmp=NULL,*child=NULL;
    int num=-1;
    int i=0;
    //printf("cmd=%s\n",cmd);
    ptr = strchr(cmd,'.');

    //judge if is #num
    if(*cmd == '#'){
        num=atoi(++cmd);
        if(root->type != cJSON_Array || cJSON_GetArraySize(root) < num){
            printf("cant find valid array or array size err!\n");
            return -1;
        }
        //printf("array size :%d\n",cJSON_GetArraySize(root));
        child= root->child;
        for(i=0;i<num;i++){
            child=child->next;
        }
    }

    if(ptr== NULL)//find the last
    {
        if(child) tmp=child;
        else
        {
            tmp = cJSON_GetObjectItem(root,cmd);
            if(!tmp){
                printf("canot find object name '%s'\n",cmd);
                return -1;
            }
        }
        if(better_print)
            ptr = cJSON_Print_b(tmp);
        else
            ptr = cJSON_Print(tmp);
        printf("%s\n",ptr);
        free(ptr);
    }else{
        *ptr = '\0';
        if(child) 
            tmp=child;
        else
        {
            tmp = cJSON_GetObjectItem(root,cmd);
            if(!tmp){
                printf("canot find object name '%s'\n",cmd);
                return -1;
            }
        }
        int ret = parase_object(tmp,++ptr);
        if(ret != 0)
            return ret;
    }
    return 0;
}

void help()
{
    printf("jsonshow(%s),build time:20180813\n",VERSION);
    printf("\t-s 'json string',cant use -f option\n");
    printf("\t-f 'json file path',cant use -s option\n");
    printf("\t-c 'parase cmd',ex:root.name root.info.user(must be start with root)\n");
    printf("\t-b '0/1',print beauty,this is useful for shell script(default 1)\n");
    printf("\tex:jsonshow -s '{\"name\":\"jack\"}' -c root.name\n");
    printf("\t   jsonshow -s '{\"name\":\"jack\",\"arr\":[1,2,3,4]}' -c root.arr.#3\n");
    printf("\t   jsonshow -f test.json -c root.links.#1.name (#num for array type,first is #0)\n");
    printf("\t   jsonshow -f test.json -c root.stu.jack.age\n");
    printf("copywrite (c) jk110333@126.com\n");
    exit(0);
}

int main(int argc,char ** argv)
{
    int ch=0;
    char *cmd = NULL;
    char *json_file=NULL;
    char *json_string=NULL;
    int err=0;
    cJSON *root=NULL;
    char *ptr=NULL;

    while ((ch = getopt(argc, argv, "c:f:s:b:h")) != -1)
    {
        switch (ch) 
        {
            case 'c':
                cmd = strdup(optarg);
                break;
            case 'b':
                better_print = atoi(optarg);
                break;
            case 'f':
                json_file = strdup(optarg);
                break;
            case 's':
                json_string = strdup(optarg);
                break;
            case 'h':
            default:
                help();
                break;
        }
    }
    if(json_file)
    {
        if(json_string){ 
            //file first
            free(json_string);
        }

        FILE *f=NULL;
        long len;

        f=fopen(json_file,"rb");
        if(!f){
            printf("open %s fail\n",json_file);
            err = 1;
            goto clean;
        }
        fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
        json_string=(char*)malloc(len+1);
        if(!json_string){
            printf("malloc %ld byte fail\n",len+1);
            err = 2;
            goto clean;
        }
        memset(json_string,0,len+1);
        fread(json_string,1,len,f);
        fclose(f);
    }
    if(!json_string){
        help();
    }
    root = cJSON_Parse(json_string);
    if (!root)
    {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        err = 3;
        goto clean;
    }

    if(strncmp(cmd,"root",4)!= 0){
        err = 4;
        goto clean;
    }
    ptr = strchr(cmd,'.');
    if(ptr== NULL)
    {
        ptr = cJSON_Print_b(root);
        printf("%s\n",ptr);
        //show all json string
        goto clean;
    }
    ptr++;
    //printf("ptr=%s\n",ptr);
    parase_object(root,ptr);
clean:
    if(root)
        cJSON_Delete(root);
    if(json_string)
        free(json_string);
    if(json_file) free(json_file);
    if(cmd) free(cmd);
    return err;
}
