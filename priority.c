#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node
{
    int pid;
    int pri;
    int at;
    int burnum;
    int bt[15];
    struct node* next;
}node;

//#define prog 10;
void createprocess(int n);
void process();
static int timer = 0;
static int prevtimer[10] = {0,0,0,0,0,0,0,0,0,0};
static int cpuwait[10] = {0,0,0,0,0,0,0,0,0,0};
static int processburstcount = 0;
static int pburcounter[10] = {0,0,0,0,0,0,0,0,0,0};
static int piotimer[10] = {0,0,0,0,0,0,0,0,0,0};
static int indprocessbur[10] = {0,0,0,0,0,0,0,0,0,0};
static int val[10] =  {0,0,0,0,0,0,0,0,0,0};
static int waitready[10] = {0,0,0,0,0,0,0,0,0,0};
static int iotime[10] = {0,0,0,0,0,0,0,0,0,0};
node* head = NULL;
node* headtoblock = NULL;
node* headtorunning = NULL;
node* headtoready = NULL;
node* headtocomplete = NULL;
node* headtoarr = NULL;
int main()
{
    printf("enter the number of processes\n");
    int n;
    scanf("%d", &n);
    createprocess(n);
    process();
    return 0;
}
void process()
{
    int i, j, checkburstcount = 0;
    int z;
    node* temp;
    node* curr;
    node* temparr = headtoarr;
    while(checkburstcount < processburstcount)
    {
        printf("------------------------------FOR TIME: %d - %d----------------------------------\n", timer, timer+1,checkburstcount, processburstcount);
        if(timer == 0)
        {
            printf("%d: STS is running\n\n",timer);
            timer++;
            continue;
        }
        //dealing with block

        /**
        on every time increment

        run process increment by one
        if process in ready and its pburcount is even add to run
        if completed its run time add to ready
        or


        decrement the block counter of process in block itself and if block less than 0 add to arrival
        add process from arrival to block
        add process to arrival if their time is less than or equal
        **/
/************************************************managing running*************************************************************/


        //node* travrun = headtorunning;
        //node* currrun = headtorunning;
        if(headtorunning != NULL)
        {
            int counter = pburcounter[headtorunning->pid];
            printf("PROCESS IN RUNNING STATE\n");
            printf("process %d for %d\n", headtorunning->pid, headtorunning->bt[counter]);
            headtorunning->bt[counter]--;
            timer++;
            node* travblock0 = headtoblock;
            node* currblock0 = headtoblock;
            while(travblock0 != NULL)
            {
                int counter = pburcounter[travblock0->pid];
                travblock0->bt[counter]--;
                travblock0 = travblock0->next;
            }
            if(headtorunning->bt[counter] <= 0)
            {
                
                printf("(process %d removed from running)\n", headtorunning->pid);
                checkburstcount++;
                pburcounter[headtorunning->pid]++;
                indprocessbur[headtorunning->pid]--;
                node* newnode2 = malloc(sizeof(node));
                if(headtoblock == NULL)
                {
                    headtoblock = newnode2;
                    newnode2->pid = headtorunning->pid;
                    newnode2->at = headtorunning->at;
                    newnode2->pri = headtorunning->pri;
                    newnode2->burnum = headtorunning->burnum;
                    int len = (2*newnode2->burnum) + 1;
                    int z;
                    for(z = 0; z < len; z++)
                    {
                        newnode2->bt[z] = headtorunning->bt[z];
                    }
                    newnode2->next = NULL;
                }
                else
                {
                    node* travblock2 = headtoblock;
                    node* currblock2 = headtoblock;
                    while(travblock2 != NULL)
                    {
                        currblock2 = travblock2;
                        travblock2 = travblock2->next;
                    }

                    currblock2->next = newnode2;
                    newnode2->pid = headtorunning->pid;
                    newnode2->at = headtorunning->at;
                    newnode2->pri = headtorunning->pri;
                    newnode2->burnum = headtorunning->burnum;
                    int len = (2*newnode2->burnum) + 1;
                    int z;
                    for(z = 0; z < len; z++)
                    {
                        newnode2->bt[z] = headtorunning->bt[z];
                    }
                    newnode2->next = NULL;
                }

                headtorunning = NULL;

            }
        }
        else{




/**************************************************adding from blocked to ready************************************************/
        //printing readylist
        printf("PROCESS IN READY STATE\n");
        node* travready = headtoready;
        while(travready != NULL)
        {
            printf("process %d, with priority %d\n", travready->pid, travready->pri);
            waitready[travready->pid] = waitready[travready->pid] + 1;
            travready = travready->next;
        }
        printf("\n");


        node* travblock1 = headtoblock;
        node* currblock1 = headtoblock;
        while(travblock1 != NULL)
        {
            node* newnode =  malloc(sizeof(node));
            int counter = pburcounter[travblock1->pid];
            if(travblock1->bt[counter] <= 0)
            {
                printf("(process %d removed from blocked)\n", travblock1->pid);
                checkburstcount++;
                pburcounter[travblock1->pid]++;
                if(indprocessbur[travblock1->pid] <= 0)
                {
                    printf("PROCESS %d COMPLETED!\n", travblock1->pid);
             	    if(headtocomplete == NULL)
                    {
		            headtocomplete = newnode;
		            newnode->pid = travblock1->pid;
		            newnode->at = travblock1->at;
		            newnode->pri = travblock1->pri;
		            newnode->burnum = travblock1->burnum;
		            int len = (2*newnode->burnum) + 1;
		            int z;
		            for(z = 0; z < len; z++)
		            {
		                newnode->bt[z] = travblock1->bt[z];
		            }
		            newnode->next = NULL;
                    }
                    else
                    {
		            node* travtempcomp = headtocomplete;
		            node* travcurrcomp = headtocomplete;
		            while(travtempcomp != NULL)
		            {
		                travcurrcomp = travtempcomp;
		                travtempcomp = travtempcomp->next;
		            }
		            travcurrcomp->next = newnode;
		            newnode->pid = travblock1->pid;
		            newnode->at = travblock1->at;
		            newnode->pri = travblock1->pri;
		            newnode->burnum = travblock1->burnum;
		            int len = (2*newnode->burnum) + 1;
		            int z;
		            for(z = 0; z < len; z++)
		            {
		                newnode->bt[z] = travblock1->bt[z];
		            }
		            newnode->next = NULL;
                    }       
                }
                else
                {
                	val[travblock1->pid] = 2;
		        if(headtoready == NULL)
		        {
		            headtoready = newnode;
		            newnode->pid = travblock1->pid;
		            newnode->at = travblock1->at;
		            newnode->pri = travblock1->pri;
		            newnode->burnum = travblock1->burnum;
		            int len = (2*newnode->burnum) + 1;
		            int z;
		            for(z = 0; z < len; z++)
		            {
		                newnode->bt[z] = travblock1->bt[z];
		            }
		            newnode->next = NULL;
		        }
		        else
		        {
		            node* travtempready = headtoready;
		            node* travcurrready = headtoready;
		            while(travtempready != NULL)
		            {
		                travcurrready = travtempready;
		                travtempready = travtempready->next;
		            }
		            travcurrready->next = newnode;
		            newnode->pid = travblock1->pid;
		            newnode->at = travblock1->at;
		            newnode->pri = travblock1->pri;
		            newnode->burnum = travblock1->burnum;
		            int len = (2*newnode->burnum) + 1;
		            int z;
		            for(z = 0; z < len; z++)
		            {
		                newnode->bt[z] = travblock1->bt[z];
		            }
		            newnode->next = NULL;
		        }
                }
                if(travblock1 == headtoblock)
                {
                    headtoblock = travblock1->next;
                }
                else
                {
                    currblock1->next = travblock1->next;
                }
            }

            currblock1 = travblock1;
            travblock1 = travblock1->next;
        }
/*******************************************************adding from blocked to ready********************************************/

        /****dealing with block****/
        node* travblock = headtoblock;
        node* currblock = headtoblock;
        printf("PROCESS IN BLOCKED STATE\n");
        while(travblock != NULL)
        {
            int counter = pburcounter[travblock->pid];
            printf("process %d for %d\n", travblock->pid, travblock->bt[counter]);
            travblock = travblock->next;
        }
        //printf("\n");

        node* travarr = headtoarr;
        node* currarr = headtoarr;

        while(travarr != NULL)
        {
            int counter = pburcounter[travarr->pid];
            printf("(process %d is added to blocked for %d)\n", travarr->pid, travarr->bt[counter]);
            //checkburstcount++;
            indprocessbur[travarr->pid]--;
            //printf("process %d burst left: %d\n", travarr->pid, indprocessbur[travarr->pid]);
            node* newnode =  malloc(sizeof(node));
            if(headtoblock == NULL)
            {
                headtoblock = newnode;
                newnode->pid = travarr->pid;
                newnode->at = travarr->at;
                newnode->pri = travarr->pri;
                newnode->burnum = travarr->burnum;
                int len = (2*newnode->burnum) + 1;
                int z;
                for(z = 0; z < len; z++)
                {
                    newnode->bt[z] = travarr->bt[z];
                }
                newnode->next = NULL;
            }
            else
            {
                node* travtempblock = headtoblock;
                node* travcurrblock = headtoblock;
                while(travtempblock != NULL)
                {
                    travcurrblock = travtempblock;
                    travtempblock = travtempblock->next;
                }
                travcurrblock->next = newnode;
                newnode->pid = travarr->pid;
                newnode->at = travarr->at;
                newnode->pri = travarr->pri;
                newnode->burnum = travarr->burnum;
                int len = (2*newnode->burnum) + 1;
                int z;
                for(z = 0; z < len; z++)
                {
                    newnode->bt[z] = travarr->bt[z];
                }
                newnode->next = NULL;
            }
            if(travarr == headtoarr)
            {
                headtoarr = travarr->next;
            }
            else
            {
                currarr->next = travarr->next;
            }
            currarr = travarr;
            travarr = travarr->next;
        }


        //adding to arrived
        printf("PROCESS ARRIVED\n");
        temp = head;
        curr = head;
        while(temp!=NULL)
        {
                    if(temp->at <= timer)
                    {
                        printf("(process %d has arrived at %d)\n", temp->pid, timer);
                        node* newnode =  malloc(sizeof(node));
                        if(headtoarr == NULL)
                        {
                            headtoarr = newnode;
                            newnode->pid = temp->pid;

                            newnode->at = temp->at;
                            newnode->pri = temp->pri;
                            newnode->burnum = temp->burnum;
                            int len = (2*newnode->burnum) + 1;
                            int z;
                            for(z = 0; z < len; z++)
                            {
                                newnode->bt[z] = temp->bt[z];
                            }
                            newnode->next = NULL;
                        }
                        else
                        {
                            node* travtemp = headtoarr;
                            node* travcurr = headtoarr;
                            while(travtemp != NULL)
                            {
                                travcurr = travtemp;
                                travtemp = travtemp->next;
                            }
                            travcurr->next = newnode;
                            newnode->pid = temp->pid;
                            newnode->at = temp->at;
                            newnode->pri = temp->pri;
                            newnode->burnum = temp->burnum;
                            int len = (2*newnode->burnum) + 1;
                            int z;
                            for(z = 0; z < len; z++)
                            {
                                newnode->bt[z] = temp->bt[z];
                            }
                            newnode->next = NULL;
                        }
                        if(temp == head)
                        {
                            head = temp->next;
                        }
                        else
                        {
                            curr->next = temp->next;
                        }
                    }
                    curr = temp;
                    temp = temp->next;

        }
        //checkburstcount += 1;
        printf("\n\n");
    /************************************************adding from ready to running**************************************************/
        //printing running
        printf("PROCESS IN RUNNING STATE\n");
        node* travrunning = headtorunning;
        while(travrunning != NULL)
        {
            printf("process %d\n", travrunning->pid);
            travrunning = travrunning->next;
        }
        printf("\n");

        node* newnode0 = malloc(sizeof(node));
        //node* testtemp = headtoready;
        int min, min0;
        node* travready0 = headtoready;
        node* currready0 = headtoready;
        if(headtoready != NULL){
        min = headtoready->pri;
        while(travready0 != NULL)
        {
        	if(travready0->pri <= min)
        	{
        		min = travready0->pri;
        		min0 = travready0->pid;
        	}
        	currready0 = travready0;
        	travready0 = travready0->next;
        }
        }
        if(headtoready != NULL && val[min0] <= 0){
        travready0 = headtoready;
        currready0 = headtoready;
        while(travready0 != NULL)
        {
        	if(travready0->pri == min)
        	{
        		printf("(process %d will be added to running state)\n", travready0->pid);
			    cpuwait[travready0->pid] = cpuwait[travready0->pid] + (timer - prevtimer[travready0->pid]);
			    prevtimer[travready0->pid] = timer;
			    indprocessbur[travready0->pid]--;
			    //checkburstcount++;
			    headtorunning = newnode0;
			    newnode0->pid = travready0->pid;
			    newnode0->at = travready0->at;
			    newnode0->pri = travready0->pri;
			    newnode0->burnum = travready0->burnum;
			    int len = (2*newnode0->burnum) + 1;
			    int z;
			    for(z = 0; z < len; z++)
			    {
				    newnode0->bt[z] = travready0->bt[z];
			    }
			    newnode0->next = NULL;
			    if(travready0 == headtoready)
			    {
			    	headtoready = travready0->next;
			    }
			    else
			    {
			    currready0->next = travready0->next;
			    }
			    break;
		}
        	currready0 = travready0;
        	travready0 = travready0->next;
        }
        }
       /**if(headtoready != NULL && val[headtoready->pid] <= 0)
        {

            printf("(process %d added to running state)\n", headtoready->pid);
            cpuwait[headtoready->pid] = cpuwait[headtoready->pid] + (timer - prevtimer[headtoready->pid]);
            prevtimer[headtoready->pid] = timer;
            indprocessbur[headtoready->pid]--;
            //checkburstcount++;
            headtorunning = newnode0;
            newnode0->pid = headtoready->pid;
            newnode0->at = headtoready->at;
            newnode0->burnum = headtoready->burnum;
            int len = (2*newnode0->burnum) + 1;
            int z;
            for(z = 0; z < len; z++)
            {
                    newnode0->bt[z] = headtoready->bt[z];
            }
            newnode0->next = NULL;

            headtoready = headtoready->next;
        }**/
	
	printf("COMPLETED PROCESS\n");
        node* travcomp = headtocomplete;\
        
        while(travcomp != NULL)
        {
            printf("process %d, total wait time in ready: %d, total wait time between CPU bursts: %d, Time %d spent on IO\n", travcomp->pid, waitready[travcomp->pid], cpuwait[travcomp->pid], iotime[travcomp->pid]);
            travcomp = travcomp->next;
        }
        printf("\n");

/**
    node* temtes = headtoarr;
    while(temtes != NULL)
    {
        int j;
        printf("%d %d %d", temtes->pid, temtes->at, temtes->burnum);
        for(j = 0; j < 2*temtes->burnum + 1; j++)
        {
            printf("%d ", temtes->bt[j]);
        }
        printf("\n");
        temtes=temtes->next;
    }
    printf("block\n\n");
    temtes = headtoblock;
    while(temtes != NULL)
    {
        int j;
        printf("%d %d %d", temtes->pid, temtes->at, temtes->burnum);
        for(j = 0; j < 2*temtes->burnum + 1; j++)
        {
            printf("%d ", temtes->bt[j]);
        }
        printf("\n");
        temtes=temtes->next;
    }
**/
    node* travblocke = headtoready;
    node* currblocke = headtoready;
    while(travblocke != NULL)
    {
        val[travblocke->pid] -= 2;
        travblocke = travblocke->next;
    }
	
    /**dealing with timer ***/
    timer++;
    node* travblock0 = headtoblock;
    node* currblock0 = headtoblock;
    while(travblock0 != NULL)
    {
        int counter = pburcounter[travblock0->pid];
        travblock0->bt[counter]--;
        travblock0 = travblock0->next;
    }
    /**dealing with timer **/
    }
    }
}

void createprocess(int n)
{
    int i, j;
    int s = time(NULL);
    srand(s);
    for(i = 0; i < n; i++)
    {
        node* newnode = malloc(sizeof(node));
        if(head == NULL)
        {

            newnode->pid = i;
            newnode->pri = rand() % n;
            newnode->at = 0;
            newnode->burnum = (rand() % 3) + 1;
            
            int len = (2 * newnode->burnum) + 1;
            processburstcount = processburstcount + len;
            indprocessbur[newnode->pid] = len;
            for(j = 0; j < len; j++)
            {
            	
                newnode->bt[j] = (rand() % 5) + 2;
                if(j % 2 == 0)
            	{
            		iotime[i] = iotime[i] + newnode->bt[j]; 
            	}
            }

            newnode->next = NULL;
            head = newnode;
        }
        else
        {
            node* temp = head;
            node* curr = head;
            while(temp != NULL)
            {
                curr = temp;
                temp = temp->next;
            }
            curr->next = newnode;
            newnode->pid = i;
            newnode->pri = rand() % n;
            newnode->at = curr->at + (rand() % 5) + 1;
            newnode->burnum = (rand() % 3) + 1;
            int len = (2 * newnode->burnum) + 1;
            indprocessbur[newnode->pid] = len;
            processburstcount = processburstcount + len;
            for(j = 0; j < len; j++)
            {
                newnode->bt[j] = (rand() % 5) + 2;
                if(j % 2 == 0)
            	{
            		iotime[i] = iotime[i] + newnode->bt[j]; 
            	}
            }
            newnode->next = NULL;
        }

    }
    printf("pid\tat\tpri\tbnum\tnoofbur\n");
    node* temp = head;
    while(temp != NULL)
    {
        printf("%d\t%d\t%d\t%d\t",temp->pid,temp->at,temp->pri,temp->burnum);
        int len = (2 * temp->burnum) + 1;
        for(j = 0; j < len; j++)
        {
            printf("%d ",temp->bt[j]);
        }
        temp = temp->next;
        printf("\n");
    }

}
