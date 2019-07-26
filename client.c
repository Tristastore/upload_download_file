#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int select_mode;
	int ret;
	DIR *dp;
	struct dirent *dir;
	int src_fd,des_fd;
	char server[128]={0};
	char file[128]={0};
	char buf[128]={0};
	
	
	start:
	printf("please input server PATH and Direactory name\n");
	scanf("%s",server);
	
	//list server files
	dp=opendir(server);
	if(dp==NULL)
	{
		printf("open server: %s failure\n",server);
		goto start;
	}
	printf("open server: %s success\n",server);
	
	while(1)
	{
		dir=readdir(dp);
		if(dir==NULL)
		{
			break;
		}
		else
		{
			printf("inode=%ld\t file_name:%s\n",dir->d_ino,dir->d_name);
		}
	}
	
	select:
	printf("select mode:\n1\tdownload\n2\tupload\n");
	scanf("%d",&select_mode);
	
	switch(select_mode)
	{
		case 1:
		{
			printf("please input download file\n");
			scanf("%s",file);
			
			//open src file
			src_fd=open(strcat(strcat(server,"/"),file),O_RDONLY);
			if(src_fd<0)
			{
				printf("open download file %s failure\n",file);
				return -1;
			}
			printf("open download file %s success\n",file);
			
			//creat des file
			des_fd=open(file,O_CREAT|O_WRONLY);
			if(des_fd<0)
			{
				printf("creat download file %s failure\n",file);
				return -2;
			}
			printf("creat download file %s success\n",file);
			
			while(1)
			{
				ret=read(src_fd,buf,128);
				if(ret<128)
				{
					break;
				}
				write(des_fd,buf,ret);
			}
			write(des_fd,buf,ret);
			break;
		}
		
		case 2:
		{
			printf("please input upload file\n");
			scanf("%s",file);
			
			//open src file
			src_fd=open(file,O_RDONLY);
			if(src_fd<0)
			{
				printf("open upload file %s failure\n",file);
				return -1;
			}
			printf("open upload file %s success\n",file);
			
			
			//creat des file
			des_fd=open(strcat(strcat(server,"/"),file),O_TRUNC|O_CREAT|O_WRONLY);
			if(des_fd<0)
			{
				printf("creat upload file %s failure\n",file);
				return -2;
			}
			printf("creat upload file %s success\n",file);
			
			
			while(1)
			{
				ret=read(src_fd,buf,128);
				if(ret<128)
				{
					break;
				}
				write(des_fd,buf,ret);
			}
			write(des_fd,buf,ret);
			break;
		}
		
		default:
		{
			printf("please select 1 or 2 \n");
			goto select;
		}
	}
	
	
	
	
	
	close(src_fd);
	close(des_fd);
	closedir(dp);
	
	return 0;
	
	
}
