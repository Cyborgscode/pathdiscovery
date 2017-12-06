#define _GNU_SOURCE
#include <libgen.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

/* alte routine 

char *getTime(char *old ) {
	
	int len = strlen( old );
	char *time = malloc( len  );
	strncpy( time, old, len - 1 );
	return time;
}

*/

int alternative = 0;
int humanreadable = 1;
int dates = 0;
int names = 1;
int nocurse = 0;

int alt_len = 0;
int hum_len = 0;
int dat_len = 0;
int nam_len = 0;


char *getTime2( time_t *old ) {
	
	struct tm *info;
	char *buffer = malloc(80);
	info = localtime( old );

	strftime( buffer,80,"%Y-%m-%d %H:%M:%S", info);

	return buffer;
}

char *makeSize(unsigned int size) {

	char *einheiten[7];
	einheiten[0] = "Bytes";
	einheiten[1] = "kb";
	einheiten[2] = "mb";
	einheiten[3] = "gb";
	einheiten[4] = "tb";
	einheiten[5] = "eb";
	einheiten[6] = "pb";
	
	int c = 0;

//	printf("c=%d size=%u\n", c, size);

	while ( ( size / 1024 ) > 1024 ) {
		c++;
		size = size / 1024;

//		printf("c=%d size=%u\n", c, size);
	}

	char *buffer = malloc( 10001 );

	sprintf( buffer, "%u %s", size, einheiten[c] );

	return buffer;

}

void fileInfo(char* path) {
	struct stat info;

//	printf("AP path=%s\n",path);
	
	if ( lstat( path, &info ) == 0 ) {

		int i=0;
		char *type   = malloc( 50 );
		char *buffer = malloc( 20 );

		if ( S_ISLNK(info.st_mode) != 0 ) {
			sprintf(buffer+0, "l" );
		} else if ( S_ISDIR(info.st_mode) != 0 ) {
			sprintf(buffer+0, "d" );
		} else  sprintf(buffer+0, "-" );

		sprintf(buffer+1, (info.st_mode & S_IRUSR) ? "r" : "-");
		sprintf(buffer+2, (info.st_mode & S_IWUSR) ? "w" : "-");
		sprintf(buffer+3, (info.st_mode & S_IXUSR) ? "x" : "-");
		sprintf(buffer+4, (info.st_mode & S_IRGRP) ? "r" : "-");
		sprintf(buffer+5, (info.st_mode & S_IWGRP) ? "w" : "-");
		sprintf(buffer+6, (info.st_mode & S_IXGRP) ? "x" : "-");
		sprintf(buffer+7, (info.st_mode & S_IROTH) ? "r" : "-");
		sprintf(buffer+8, (info.st_mode & S_IWOTH) ? "w" : "-");
		sprintf(buffer+9, (info.st_mode & S_IXOTH) ? "x" : "-");

		switch (info.st_mode & S_IFMT) {
		    case S_IFBLK:  strcpy(type,"block device");            break;
		    case S_IFCHR:  strcpy(type,"character device");        break;
		    case S_IFREG:  strcpy(type,"regular file");            break;
		    case S_IFLNK:  strcpy(type,"symlink");                 break;
		    case S_IFDIR:  strcpy(type,"directory");               break;
		    case S_IFIFO:  strcpy(type,"FIFO/pipe");               break;
		    case S_IFSOCK: strcpy(type,"socket");                  break;
		    default:       strcpy(type,"** unknown **");           break;
		}


		char *altbuffer = malloc(1000);
		char *datebuffer = malloc(500);
		char *sizebuffer = malloc(500);
		char *namebuffer = malloc(500);
		char *ids;

		if ( alternative == 0 ) {
			sprintf( altbuffer, "%s", basename(path) );
		} else {
			sprintf( altbuffer, "%s", path );
		}

		if ( humanreadable == 0 ) {
			sprintf( sizebuffer, "%10u Byte", info.st_size );
		} else {
			sprintf( sizebuffer, "%12s", makeSize(info.st_size) );
		}

		if ( dates == 0 ) {
			sprintf( datebuffer, " " );
		} else {
			sprintf( datebuffer, " (%s | %s | %s) ",
 			       getTime2( &info.st_ctime ), 
			       getTime2( &info.st_mtime ),
	                       getTime2( &info.st_atime ) );
		}

		if ( names == 0 ) {
			ids = malloc(20);
			sprintf( ids,"%d/%d", info.st_uid, info.st_gid );

		} else {
			ids = malloc(500); 
			struct passwd *pwd;
			struct group *grp;

			pwd = getpwuid( info.st_uid);
			grp = getgrgid( info.st_gid);
			
			sprintf( ids,"%s/%s", pwd->pw_name, grp->gr_name );
		}

		int ilen = strlen( ids );
		for(i=ilen;i<nam_len;i++) strcat(ids," ");
		ilen = strlen( datebuffer );
		for(i=ilen;i<dat_len;i++) strcat(datebuffer," ");
		ilen = strlen( altbuffer );
		for(i=ilen;i<alt_len;i++) strcat(altbuffer," ");
		ilen = strlen( sizebuffer );
		for(i=ilen;i<hum_len;i++) strcat(sizebuffer," ");
		
/*
		alt_len = strlen(altbuffer);
		hum_len = strlen(sizebuffer);
		dat_len = strlen(datebuffer);
		nam_len = strlen(ids);
*/


		printf("%s %s%s %s : %s ( %s )\n",sizebuffer, datebuffer, ids, buffer, altbuffer , type );
	}
}

void fileInfo_test(char* path) {
	struct stat info;

	if ( lstat( path, &info ) == 0 ) {

		char *type   = malloc( 50 );
		char *altbuffer = malloc(1000);
		char *datebuffer = malloc(500);
		char *sizebuffer = malloc(500);
		char *namebuffer = malloc(500);
		char *ids;

		if ( alternative == 0 ) {
			sprintf( altbuffer, "%s", basename(path) );
		} else {
			sprintf( altbuffer, "%s", path );
		}

		if ( humanreadable == 0 ) {
			sprintf( sizebuffer, "%u Byte", info.st_size );
		} else {
			sprintf( sizebuffer, "%s", makeSize(info.st_size) );
		}

		if ( dates == 0 ) {
			sprintf( datebuffer, " " );
		} else {
			sprintf( datebuffer, " (%s | %s | %s) ",
 			       getTime2( &info.st_ctime ), 
			       getTime2( &info.st_mtime ),
	                       getTime2( &info.st_atime ) );
		}

		if ( names == 0 ) {
			ids = malloc(20);
			sprintf( ids,"%d/%d", info.st_uid, info.st_gid );

		} else {
			ids = malloc(500); 
			struct passwd *pwd;
			struct group *grp;

			pwd = getpwuid( info.st_uid);
			grp = getgrgid( info.st_gid);
			
			sprintf( ids,"%s/%s", pwd->pw_name, grp->gr_name );
		}

		if ( nocurse == 0 ) {

			if ( strlen(altbuffer) > alt_len ) alt_len = strlen(altbuffer);
			if ( strlen(sizebuffer) > hum_len ) hum_len = strlen(sizebuffer);
			if ( strlen(datebuffer) > dat_len ) dat_len = strlen(datebuffer);
			if ( strlen(ids) > nam_len ) nam_len = strlen(ids);

		}

	}
}


int analysePath(char* path) {

	fileInfo( path );

	char *dir = dirname(path);	
	char *file = basename(path);

	if ( strcmp(".",dir)==0 || strcmp("/",dir)==0 ) return 0;
	return analysePath( dir );
}

int analysePath_test(char* path) {

	fileInfo_test( path );

	char *dir = dirname(path);	
	char *file = basename(path);

	if ( strcmp(".",dir)==0 || strcmp("/",dir)==0 ) return 0;
	return analysePath_test( dir );
}



int analyse(char *path) {

	FILE *file;
	if ( file = fopen( path, "r") ) {
	        fclose(file);

//		fileInfo( path );

		long size;
		char *buf;
		char *ptr;

		size = pathconf( path, _PC_PATH_MAX);
		if ((buf = (char *)malloc((size_t)size)) != NULL) {

			char *real = realpath( path, NULL );
			if ( real != NULL ) {
	
				printf("\n'%s' translates to '%s'\n\n", path, canonicalize_file_name(path) );
				analysePath( real );

				return 0;
			} else {
				printf("Can not resolve \"%s\"\n", path);
			}
		} else {
			printf("can not allocate enough memory for call getcwd()\n");
		}
	} else {
 		printf("File \"%s\" does not exist!\n", path);
	}

	return 1;
}

int analyse_test(char *path) {

	FILE *file;
	if ( file = fopen( path, "r") ) {
	        fclose(file);

		long size;
		char *buf;
		char *ptr;

		size = pathconf( path, _PC_PATH_MAX);
		if ((buf = (char *)malloc((size_t)size)) != NULL) {

			char *real = realpath( path, NULL );
			if ( real != NULL ) {
				analysePath_test( real );
			}
		}
	}

	return 0;
}



int main(int argc, char *argv[]) {

	if ( argc < 2 ) {
		printf("Usage: pathdiscover [-a] [-d] [-N] [-n] [-V] [-NC] <filename>\n");
		return 1;
	}

	int c = argc;
	int i = 0;
	for(i=1;i<argc;i++) {
		if ( strcmp(argv[i],"-V")==0 ) {
			printf("0.1a\n");
			return 0;
		} 
		if ( strcmp(argv[i],"-n")==0 || strcmp(argv[i],"--names")==0 ) {
			names = 0;
			c--;
		} else if ( strcmp(argv[i],"-N")==0 || strcmp(argv[i],"--numbers")==0 ) {
			humanreadable = 0;
			c--;
		} else if ( strcmp(argv[i],"-a")==0 || strcmp(argv[i],"--alternative")==0 ) {
			alternative = 1;
			c--;
		} else if ( strcmp(argv[i],"-d")==0 || strcmp(argv[i],"--full-time")==0 ) {
			dates = 1;
			c--;
		} else if ( strcmp(argv[i],"-NC")==0 ) {
			nocurse = 1;
			c--;
		} else {
			analyse_test( argv[i] );
		}
	}

	for(i=1;i<argc;i++) {
		// printf("Argument %d: %s\n",i, argv[i]);

		if ( strcmp(argv[i],"-V")==0 ) {
			printf("0.1a\n");
			return 0;
		} 
		if ( strcmp(argv[i],"-n")==0 || strcmp(argv[i],"--names")==0 ) {
			names = 0;
			c--;
		} else if ( strcmp(argv[i],"-N")==0 || strcmp(argv[i],"--numbers")==0 ) {
			humanreadable = 0;
			c--;
		} else if ( strcmp(argv[i],"-a")==0 || strcmp(argv[i],"--alternative")==0 ) {
			alternative = 1;
			c--;
		} else if ( strcmp(argv[i],"-d")==0 || strcmp(argv[i],"--full-time")==0 ) {
			dates = 1;
			c--;
		} else if ( strcmp(argv[i],"-NC")==0 ) {
			nocurse = 1;
			c--;
		} else {
			analyse( argv[i] );
			if ( c > 2 ) printf("\n");
		}
	}

	return 1;
}


