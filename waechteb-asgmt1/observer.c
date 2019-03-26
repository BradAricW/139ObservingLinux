#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//...
int main(int argc, char *argv[]) {
//...
	int interval = 0; 
	int iteration = 0;
	int duration = 0;
	int reportType;
	char repTypeName[16];
	char c1, c2;
	struct timeval now;
	int LB_SIZE; 
	char lineBuf[255];
	FILE *thisProcFile;
	void sampleLoadAvg();
//...
	/* Determine report type */
	reportType = 0;
	strcpy(repTypeName, "Standard");
	
	//PART B BEGIN
	//model
	fprintf(stderr, "\n");
	system("grep -m1 \"model name\" /proc/cpuinfo");
	
	//kernel
	fprintf(stderr, "\nkernel version  : ");
	system("uname -r");
	fprintf(stderr, "\n");

	//last access 
	fprintf(stderr, "last accessed   : (dd/hh/mm/ss) ");
	system("uptime | awk -F\'( |,|:)+\' \'{print $6\":\"$2\":\" $3\":\" $4}\'");
	
	
		
	if (argc > 1) {
		sscanf(argv[1], "%c%c", &c1, &c2);
		if (c1 != '-') {
			fprintf(stderr, "usage: observer [-s][-l int dur]\n");
			exit(1);
		}
		if (c2 == 's') {
			reportType = 1;
			strcpy(repTypeName, "Short");
			//PART C BEGIN
			//user mode, system mode, idle
			fprintf(stderr, "\n");
			system("top | grep -m1 \"Cpu\" | awk -F'{ |,|  |%|}+' '{print \"user time       :  \" $2 \"%  \"}'");
			system("top | grep -m1 \"Cpu\" | awk -F'{ |,|  |%|}+' '{print \"system time     :  \" $5 \"%  \"}'");
			system("top | grep -m1 \"Cpu\" | awk -F'{ |,|  |%|}+' '{print \"idle time       : \" $10\"%  \"}'");
			fprintf(stderr, "\n");
			
			//disk requests, context switches
			system("iostat -m | grep \"Device\"");
			system("iostat -m | grep \"sda\"");
			fprintf(stderr, "\n");
			system("grep ctxt /proc/12/status");
			fprintf(stderr, "\n");
			
			//time and proccesses of last boot
			fprintf(stderr, "last boot time  : ");
			system("last boot");
			fprintf(stderr, "\n# of processes  : ");
			system("cat /proc/stat | grep \"processes\"");
		}
		if (c2 == 'l') {
			reportType = 2;
			strcpy(repTypeName, "Long");
			interval = atoi(argv[2]);
			duration = atoi(argv[3]);
			
			//PART D BEGIN
			fprintf(stderr, "\n");
			system("cat /proc/meminfo | grep \"MemTotal:\"");
			fprintf(stderr, "\n");
			system("cat /proc/meminfo | grep \"MemFree:\"");
			fprintf(stderr, "\n");
		}
	}

	while (iteration < duration) {
		sleep(interval);
		sampleLoadAvg();
		iteration += interval;
	}
	fprintf(stderr, "\n");
	
	/* Finish initialization */
	gettimeofday(&now, NULL); // Get the current time
	printf("Status rept type: %s at %s\n", repTypeName, ctime(&(now.tv_sec)));

	/* Get the host filename and print it */
	thisProcFile = fopen("/proc/sys/kernel/hostname", "r");
	fgets(lineBuf, sizeof(lineBuf)/*LB_SIZE+1*/, thisProcFile);
	printf("Machine hostname: %s", lineBuf);
	fclose(thisProcFile);
	fprintf(stderr, "\n");
	
	exit(0);
}

//PART D CONTINUED
void sampleLoadAvg() {
	double la[3];  
	if (getloadavg(la, 3) != -1)
	{  
		printf("load average : %f, %f, %f\n", la[0], la[1], la[2]);
	}
}
