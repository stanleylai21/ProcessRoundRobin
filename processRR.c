#include<stdio.h>
#include<stdlib.h>

//create process structure used for the algorithm
struct Process{
	char name;
	int arrivalTime;
	int runTime;
	int priority;
	int skip;
};

//set number of processes as well as some variables used for calculating global averages
const int NUMBER_OF_PROCESSES = 25;
float turnaround = 0;
float wait = 0;
float response = 0;
float throughput = 0;

//given a process, prints its attributes (name, arrival time, runtime, priority)
void printProcess(struct Process *p)
{
	printf("Name: %c ", p->name);
	printf("Arrival Time: %i ", p->arrivalTime);
	printf("Run Time: %i ", p->runTime);
	printf("Priority: %i\n",p->priority);
}

//given an array of processes and an index, populates the attributes with relevant data
void initializeProcess(struct Process *p, int i)
{
	char charset[] = "ABCDEFGHIJLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	p->name = charset[i];
	p->arrivalTime = rand() % 100;
	p->runTime = rand() % 11;
	if (p->runTime == 0)
	{
		p->runTime++;
	}
	p->priority = rand() % 5;
	if (p->priority == 0)
	{
		p->priority++;
	}
	p->skip = 0;
}

//used for sorting the process array by arrival time
int compare(void *s1, void *s2)
{
  struct Process *p1 = (struct Process *)s1;
  struct Process *p2 = (struct Process *)s2;
  int timeDiff = p1->arrivalTime - p2->arrivalTime;
  return timeDiff;
}

//the actual algorithm, which outputs some statistics given an array of processes
void roundRobin(struct Process p[])
{
	int remainingRunTime[NUMBER_OF_PROCESSES];
	int remainingArrivalTime[NUMBER_OF_PROCESSES];
	int waitTime[NUMBER_OF_PROCESSES];
	int turnaroundTime[NUMBER_OF_PROCESSES];
	int responseTime[NUMBER_OF_PROCESSES];
	for (int i = 0;i<NUMBER_OF_PROCESSES;i++)
	{
		remainingRunTime[i] = p[i].runTime;
		remainingArrivalTime[i] = p[i].arrivalTime;
		waitTime[i] = 0;
		turnaroundTime[i] = 0;
		responseTime[i] = 0;
	}
	int time = 0;
	int finished = 0;
	int allArriving = 0;
	int finishedProcesses = 0;
	//simulates a process running for one quantum
	void run(struct Process p[], int i)
	{
		if (p[i].runTime == remainingRunTime[i])
		{
			//this continues to update responseTime array until it begins its run, giving correct value
			responseTime[i] = time - p[i].arrivalTime;
		}
		//used to check if there are no available processes to run
		allArriving = 1;
		finished = 0;
		remainingRunTime[i]--;
		if (remainingRunTime[i] == 0)
		{
			//update stats if process has completed, also sets a skip flag so that process is not run again
			turnaroundTime[i] = time - p[i].arrivalTime + 1;
			waitTime[i] = turnaroundTime[i] - p[i].runTime + 1;
			finishedProcesses++;
			p[i].skip = 1;
		}
		printf("%c", p[i].name);
		printf("%i ", time);
		time++;
		if (time == 100)
		{
			//this sets all processes that have not begun to execute to skip, meaning they cannot start after time 100
			for (int i = 0;i<NUMBER_OF_PROCESSES;i++)
			{
				if (p[i].runTime == remainingRunTime[i])
				{
					p[i].skip = 1;
				}
			}
		}
	}
	while (finished == 0 && time <= 100)
	{
		finished = 1;
		allArriving = 0;
		for (int i = 0;i<NUMBER_OF_PROCESSES;i++)
		{
			if (p[i].skip == 1)
			{
				continue;
			}
			if (remainingArrivalTime[i] > 0)
			{
				finished = 0;
				//if process is still arriving, updates arrival time
				remainingArrivalTime[i] = p[i].arrivalTime - time;
				if (remainingArrivalTime[i] == 0)
				{
					run(p, i);
				}
			}
			else if (remainingRunTime[i] > 0)
			{
				run(p, i);
			}
			else
			{
				allArriving = 1;
				time++;
			}
		}
		if(allArriving == 0)
		{
			//all processes are still arriving, so update the time
			time++;
		}
	}
	int totalTurn = 0, totalWait = 0, totalResp = 0;
	double average = 0;
	//calculate and display stats
	for (int i = 0;i<NUMBER_OF_PROCESSES;i++)
	{
		totalTurn += turnaroundTime[i];
		totalWait += waitTime[i];
		totalResp += responseTime[i];
	}
	average = totalTurn / (float)finishedProcesses;
	turnaround += average;
	printf("\nAverage Turnaround Time: ");
	printf("%f", average);
	printf("\nAverage Wait Time: ");
	average = totalWait / (float)finishedProcesses;
	wait += average;
	printf("%f", average);
	printf("\nAverage Response Time: ");
	average = totalResp / (float)finishedProcesses;
	response += average;
	printf("%f", average);
	printf("\nThroughput: ");
	average = finishedProcesses / (float)time;
	throughput += average;
	printf("%f\n", average);
}

int main()
{
	//generate random seed
	int seed = time(NULL);
	srand(seed);
	//generate new processes and run algorithm 5 times
	for (int i = 0; i<5; i++){
	struct Process processQueue[NUMBER_OF_PROCESSES];
	for (int i = 0;i < NUMBER_OF_PROCESSES;i++)
	{
		initializeProcess(&processQueue[i], i);
	}
	//sort process array using compare function, which sorts by arrival time
	qsort(processQueue, NUMBER_OF_PROCESSES, sizeof(struct Process), compare);
	for (int i = 0;i < NUMBER_OF_PROCESSES;i++)
	{
		printf("ID: %i\t",i+1);
		printProcess(&processQueue[i]);
	}
	roundRobin(processQueue);
}
	//calculate and print stats
	printf("\n\nGlobal Average Turnaround Time: ");
	printf("%f", (float)turnaround / 5);
	printf("\nGlobal Average Wait Time: ");
	printf("%f", (float)wait / 5);
	printf("\nGlobal Average Response Time: ");
	printf("%f", (float)response / 5);
	printf("\nAverage Throughput: ");
	printf("%f\n", (float)throughput / 5);
	return 0;
}
