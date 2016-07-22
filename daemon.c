#include<stdio.h>
#include<unistd.h>
#include<signal.h>
//#include<sys/param.h>
//#include<sys/stah.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

#define CMD_STR_LEN  1024
#define chartoint(x) (x-'0')




char *host = "" ;
char *file = "" ;
char *command = "";
char *start_command ="";
int  max_run = 0 ;
int  min_run = 0 ;
int  sleep_time = 1 ;



char *ret = "";
char ret_a[200];


void use_age(){

	printf("usage : -c [command] -r [restartc_command] -h [host] -m [max_run_count] -n [min_run_count] -s [sleep_time] -f [script name]\n");
	printf("example : ./daemon -r \"./test\" -m 30 -c \"ps -ef |grep test |wc -l\" \n");
	
	exit(0);

}

void init(int argc , char *argv[]){

	int i ;
	char *tmp_param ;
	for( i=0; i< argc ; i++){
			
		//printf("argv[%d] is %s \n", i ,argv[i]);
		tmp_param = argv[i] ;	 
		
			if(strcmp(argv[i] , "-h") == 0 ){  
			    host = argv[i+1];
			    i++;
			    continue;
				 }
			if(strcmp(argv[i] , "-f") == 0 ){
			    file = argv[i+1];
			    i++;
			    continue;
				 }
			if(strcmp(argv[i] , "-m") == 0 ){
			    max_run = atoi(argv[i+1]);
			    i++;
			    continue;
				 }
			if(strcmp(argv[i] , "-n") == 0 ){
			    min_run = atoi(argv[i+1]);
			    i++;
			    continue;
				 }
			if(strcmp(argv[i] , "-s") == 0 ){
			    sleep_time = atoi(argv[i+1]);
			    i++;
			    continue;
				 }
			if(strcmp(argv[i] , "-c") == 0 ){ 
			    command = argv[i+1];
			    i++;
			    continue;
				 }
		    if(strcmp(argv[i] , "-r") == 0 ){ 
			    start_command = argv[i+1];
			    i++;
			    continue;
				 }

				 
	}
	
	if(command == "")
		use_age();
	if(start_command == "")
		use_age();
	if(max_run == 0)
		use_age();
	//if(min_run == 0)
	//	use_age();

}

void excute(){



}


//----------------------------------------------------------------------------------
int mysystem_pipe(char *cmdstring, char *buf, unsigned int len)
//----------------------------------------------------------------------------------
// 增强的system函数，能够返回system调用的输出
// 增强的system函数，能够返回system调用的输出   *
// @param[in] cmdstring 调用外部程序或脚本的命令串
// @param[out] buf 返回外部命令的结果的缓冲区
// @param[in] len 缓冲区buf的长度   *
// @return 0: 成功; -1: 失败
{   
    int   fd[2];
    pid_t pid;
    int   n, count;  
    memset( buf, 0 , len);  
    if (pipe(fd) < 0)
    {   
        return -1;
    }
    if ((pid = fork()) < 0)
    { 
        return -1;
    }
    else if (pid > 0)     /* parent process */
    {   
        close(fd[1]);     /* close write end */
        count = 0;
        while ((n = read(fd[0], buf + count, len)) > 0 && count > len)
        {
            count += n;
        }
        close(fd[0]);
        if (waitpid(pid, NULL, 0) > 0)
        {  
            return -1;
        }
    }
    else                  /* child process */
    {
        close(fd[0]);     /* close read end */
        if (fd[1] != STDOUT_FILENO)
        {
            if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
            {  
                return -1;
            }
            close(fd[1]);
        }
        if (execl("/bin/sh", "sh", "-c", cmdstring, (char*)0) == -1)
        {
            return -1;
        }
    }
    ret = buf ; 
    return 0;
}


 
   int mysystem_tmpfile(char* cmdstring, char* tmpfile)
    {

        char cmd_string[CMD_STR_LEN];
        char tmpname[20] ;
        char * filename ;
        filename = tmpnam(tmpname);
        sprintf(cmd_string, "%s > %s", cmdstring, tmpname);   
        printf("cmdstring %s\n", filename);           //     /var/tmp/tmp.0.lvLEJb   临时文件
        return system(cmd_string);
    }

	int execute_popen(char* cmdstring ){

			FILE *pf = popen(cmdstring , "r");
		    char res[1024];
		    fread(res, 1024, 1, pf);
		    ret = res;
		    //printf("----%s---\n", res);
		    pclose(pf);
		    return 0;
	}

	char *  VS_StrTrim(char *pStr) {   
		     char *result;
		     char * tmp ;
		     char  tmp_a[100] ;
		     char pTmp[5]; 	
			 int i=0;
			 int j=0 ;

			    while(pStr[i]!='\0')
			    {
			    	if(pStr[i] != ' '){
			    		tmp_a[j] = pStr[i];
			    		j++;
			    		//strcpy(result, pStr[i]);     //第二个参数是char   不是char * 所以只能用c数组处理
    					//strcat(result, tmp);  
						//strcat(tmp, pStr[i]);

			    	}
			        i++;
			 
			    }

		    return  tmp_a ;
		 
		}  

void main(int argc , char * argv[]){

	init(argc , argv);
	char* tmpfile;

    //daemon(0,0);   daemon不能新启动进程 因为父进程已结束  子进程被系统回收 所以不能调用有关fork()的所有函数 system()  pipe()  fopen()  怎样防止这个进程被杀死 /etc/init.d/   superbise	
    while ( 1 )
    {
        execute_popen(command);    //ret 在此获取的 
        //printf("current fork count %d\n", (int)chartoint(VS_StrTrim(ret)));
        printf("\n\n\ncurrent fork count%d\n", atoi(VS_StrTrim(ret)));
        printf("max_run  is %d\n", max_run);
        //printf("max_run to int  is %d\n", (int)max_run);
    	if(atoi(VS_StrTrim(ret)) < max_run){

		    system(start_command);
    		printf("i am  start    %s\n", start_command);

			}
    	sleep(sleep_time);
    }


}





