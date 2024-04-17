/***************************************************
 *                                                 *
 * file: among_us.c                                *
 *                                                 *
 * @Author  Antonios Peris                         *
 * @Version 20-10-2020                             *
 * @email   csdp1196@csd.uoc.gr                    *
 *                                                 *
 * @brief   Implementation of among_us.h           *
 *                                                 *
 *************************************************
 */

#include "among_us.h"

//make players sentinel
struct Players* players_sentinel;

/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize(){
    //EVENT P
    players_head=(struct Players*)malloc(sizeof(struct Players));
    players_head = NULL;
    players_sentinel=NULL;
    
    //EVENT T
    tasks_head=(struct Head_GL*)malloc(sizeof(struct Head_GL));
	tasks_head->head=(struct Tasks*)malloc(sizeof(struct Tasks));
	tasks_head->head=NULL;
    tasks_head->tasks_count[0]=0;
    tasks_head->tasks_count[1]=0;
    tasks_head->tasks_count[2]=0;

    //EVENT I
    tasks_stack=(struct  Head_Completed_Task_Stack*)malloc(sizeof(struct  Head_Completed_Task_Stack));
    tasks_stack->head=(struct Tasks*)malloc(sizeof(struct Tasks));
    tasks_stack->count=0;
    tasks_stack->head=NULL;
    return 1;
}

/**
 * @brief Register player
 *
 * @param pid The player's id
 *
 * @param is_alien The variable that decides if he is an alien or not
 * @return 1 on success
 *         0 on failure
 */
int register_player(int pid,int is_alien) {
    struct Players* new_player=(struct Players*)malloc(sizeof(struct Players));
    struct Players* temp=(struct Players*)malloc(sizeof(struct Players));

        //for every new player
        new_player->pid=pid;
        new_player->evidence=0;
        new_player->is_alien=is_alien;
        new_player->tasks_head=NULL;

        //for player's sentinel
        if(players_head == NULL){
            //create sentinel
            temp->next=NULL;
            temp->pid=-1;
            temp->is_alien=-1;
            temp->evidence=-1;
            temp->tasks_head=NULL;
            temp->tasks_sentinel=NULL;
            players_sentinel=temp;

            //connect
            players_head = new_player;
            players_head->next = players_sentinel;
            players_head->prev = players_sentinel;
            players_sentinel->prev = players_head;
            players_sentinel->next = players_head;
        }else{
            temp = players_sentinel->prev;
            new_player->next = players_sentinel;
            players_sentinel->prev = new_player;
            new_player->prev = temp;
            temp->next = new_player;
        }
        print_players();


    return 1;
}

/**
 * @brief Insert task in the general task list
 *
 * @param tid The task id
 * 
 * @param difficulty The difficulty of the task
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_task(int tid,int difficulty) {
    struct Tasks* new_task=(struct Tasks*)malloc(sizeof(struct Tasks));
	struct Tasks* curr,*temp2;
	struct Tasks* pred;

    new_task->tid = tid;
    new_task->difficulty = difficulty;
    //new_task->next = NULL;
    if(difficulty==1){
        tasks_head->tasks_count[0]+=1;
    }else if(difficulty==2){
        tasks_head->tasks_count[1]+=1;
    }else{
        tasks_head->tasks_count[2]+=1;
    }
   
    //for first element.
    if(tasks_head->head==NULL || tasks_head->head->difficulty>=new_task->difficulty){
        new_task->next=tasks_head->head;
        tasks_head->head=new_task;
    }else{
        curr=tasks_head->head;
        while(curr->next!=NULL && curr->next->difficulty<new_task->difficulty){
            curr=curr->next;
        }
        new_task->next=curr->next;
        curr->next=new_task;
    }

    print_tasks();
    return 1;
}


int distribute_tasks(){
    int i=2;
    struct Players* pi;
    struct Tasks *ti, *point, *temp, *dummy;
    
    ti=tasks_head->head;
    pi=players_head;

    while(ti!=NULL){
        if(pi->is_alien != 0){
            pi=pi->next;
        }else{
            temp=(struct Tasks*)malloc(sizeof(struct Tasks));
            dummy=(struct Tasks*)malloc(sizeof(struct Tasks));
            if(pi->tasks_head==NULL){
                temp->tid=ti->tid;
                temp->difficulty=ti->difficulty;
                temp->next=NULL;
                pi->tasks_head=temp;
                point=pi->tasks_head;
                dummy->difficulty=0;
                dummy->tid=-1;
                pi->tasks_sentinel=dummy;
                pi->tasks_sentinel->next=NULL;
                point->next=pi->tasks_sentinel;
                point=point->next;
            }else{
                pi->tasks_sentinel->tid=ti->tid;
                pi->tasks_sentinel->difficulty=ti->difficulty;
                point=pi->tasks_sentinel;
                dummy->difficulty=0;
                dummy->tid=-1;
                pi->tasks_sentinel=dummy;
                pi->tasks_sentinel->next=NULL;
                point->next=pi->tasks_sentinel;
                point=point->next;
            }
            pi=pi->next;
            ti=ti->next;
        }
    }

    print_double_list();
}


/**
 * @brief Implement Task
 *
 * @param pid The player's id   
 *
 * @param difficulty The task's difficuly
 *
 * @return 1 on success
 *         0 on failure
 */



int implement_task(int pid, int difficulty) {
    int i=2;
    int isIttrue=0;
    struct Players* pi;
    struct Tasks* ti, *prev, *last, *elNode,  *forStack, *this, *tobedeleted;

    pi = players_head;
    //for first player---------------------------------------------------------------------------------------------------------
    if(pi->pid==pid){
        isIttrue=1;
        if(pi->pid==pid && pi->is_alien==0 && pi->tasks_head!=pi->tasks_sentinel){
            ti=pi->tasks_head;
            //if first task has difficulty #
            if(ti->difficulty==difficulty){
                this=pi->tasks_head;
                addToStack(this);
                if(pi->tasks_head->next->difficulty==0){
                    
                    pi->tasks_head=pi->tasks_sentinel;
                }else{
                    pi->tasks_head=ti->next;
                }
                free(ti);
            }else if(ti->difficulty!=difficulty && ti->next==pi->tasks_sentinel){
                this=pi->tasks_head;
                addToStack(this);
                /*elNode=(struct Tasks*)malloc(sizeof(struct Tasks));
                elNode->difficulty=0;
                elNode->difficulty=0;
                pi->tasks_head=elNode;*/
                pi->tasks_head->next=pi->tasks_sentinel;
            }else{   
                prev=ti;
                ti=ti->next;
                if(ti!=pi->tasks_sentinel){
                    while(ti!=pi->tasks_sentinel){
                        if(ti->difficulty==difficulty || ti->next==pi->tasks_sentinel){
                            tobedeleted=ti;
                            break;
                        }
                        prev=ti;
                        if(ti->next->next==pi->tasks_sentinel){
                            last=ti;
                            tobedeleted=ti->next;
                        }
                        ti=ti->next;
                    }
                    //if we didnt find the element:
                    if(ti==pi->tasks_sentinel){
                        last->next=pi->tasks_sentinel;
                        this=tobedeleted;
                        addToStack(this);;
                        free(tobedeleted);
                    }else{//else
                        prev->next=tobedeleted->next;
                        this=tobedeleted;
                        addToStack(this);;
                        free(tobedeleted);
                    }
                }
            }
        }
        //pi=pi->next;
    }
    //for the rest players-------------------------------------------------------------------------------------------------------------
    if(isIttrue==0){
        pi=pi->next;
        while(pi!=players_head){
            if(pi->pid==pid && pi->is_alien==0 && pi->tasks_head!=pi->tasks_sentinel){
                ti=pi->tasks_head;
                //if first task has difficulty #
                if(ti->difficulty==difficulty){
                    this=pi->tasks_head;
                    addToStack(this);
                    if(pi->tasks_head->next==pi->tasks_sentinel){
                        //printf("ti ginetai re\n");
                         /*elNode=(struct Tasks*)malloc(sizeof(struct Tasks));
                        elNode->difficulty=0;
                        elNode->difficulty=0;
                        pi->tasks_head=elNode;*/
                        pi->tasks_head=pi->tasks_sentinel;
                    }else{
                        pi->tasks_head=ti->next;
                    }
                    free(ti);
                }else if(ti->difficulty!=difficulty && ti->next==pi->tasks_sentinel){
                    this=pi->tasks_head;
                    addToStack(this);
                    elNode=(struct Tasks*)malloc(sizeof(struct Tasks));
                    elNode->difficulty=0;
                    elNode->difficulty=0;
                    pi->tasks_head=elNode;
                    pi->tasks_head->next=pi->tasks_sentinel;
                }else{   
                    prev=ti;
                    ti=ti->next;
                    if(ti!=pi->tasks_sentinel){
                        while(ti!=pi->tasks_sentinel){
                            if(ti->difficulty==difficulty || ti->next==pi->tasks_sentinel){
                                tobedeleted=ti;
                                break;
                            }
                            prev=ti;
                            if(ti->next->next==pi->tasks_sentinel){
                                last=ti;
                                tobedeleted=ti->next;
                            }
                            ti=ti->next;
                        }
                        //if we didnt find the element:
                        if(ti==pi->tasks_sentinel){
                            last->next=pi->tasks_sentinel;
                            this=tobedeleted;
                            addToStack(this);
                            free(tobedeleted);
                        }else{//else
                            prev->next=tobedeleted->next;
                            this=tobedeleted;
                            addToStack(this);
                            free(tobedeleted);
                        }
                    }
                }
            }
            pi=pi->next;
        }
    }
    isIttrue=0;
    print_double_list();
    return 1;
}

/**
 * @brief Eject Player
 * 
 * @param pid The ejected player's id
 *
 * @return 1 on success
 *         0 on failure
 */
int eject_player(int pid) {
    ejectAndImport(pid,0,0);
    print_double_list();
    return 1;
}

/**
 * @brief Witness Eject Player
 *
 * @param pid_a The alien's pid
 * 
 * @param pid The crewmate's pid
 * 
 * @param number_of_witnesses The number of witnesses
 *
 * @return 1 on success
 *         0 on failure
 */
int witness_eject(int pid, int pid_a, int number_of_witnesses){
    ejectAndImport(pid,pid_a,number_of_witnesses);
    print_double_list();
    return 1;
}


/**
 * @brief Sabbotage
 *
 * @param pid The player's id
 *
 * @param number_of_tasks The number of tasks to be popped
 * 
 * @return 1 on success
 *         0 on failure
 */
int sabbotage(int pid, int number_of_tasks) {
    int i=0;
    struct Tasks* stacki=tasks_stack->head, *helpy , *point, *pointhead, *ph, *hpiHead;
    struct Players* pi=players_head, *hpi;
    //printf("this is an error?\n");

    
    while(stacki!=NULL){
        helpy=(struct Tasks*)malloc(sizeof(struct Tasks));
        if(i==number_of_tasks){
            break;
        }else{
            if(i==0){
                helpy->tid=stacki->tid;
                helpy->difficulty=stacki->difficulty;
                point=helpy;
                pointhead=point;
            }else{
                helpy->tid=stacki->tid;
                helpy->difficulty=stacki->difficulty;
                point->next=helpy;
                point=point->next;
            }
            tasks_stack->head=stacki->next;
            free(stacki);
            i++;
        }
        stacki=stacki->next;
    }
    struct Tasks* curr;
    i=0;
    pi=pi->next;
    while(1){
        if(pi->pid==pid){
            hpi=pi;
            while(i<=number_of_tasks/2){
                hpi=hpi->prev;
                i++;
            }
            ph=pointhead;
            i=0;
            while(i<number_of_tasks){
                if(hpi->is_alien!=0){
                    hpi=hpi->next;
                }
                //printf("yoyoy\n");
                helpy=(struct Tasks*)malloc(sizeof(struct Tasks));
                helpy->tid=ph->tid;
                helpy->difficulty=ph->difficulty;
                if(hpi->tasks_head==hpi->tasks_sentinel || hpi->tasks_head->difficulty==0 || hpi->tasks_head->difficulty>=helpy->difficulty){
                    helpy->next=hpi->tasks_head;
                    hpi->tasks_head=helpy;
                }else{
                    curr=hpi->tasks_head;
                    while(curr->next!=hpi->tasks_sentinel && curr->next->difficulty<helpy->difficulty){
                        curr=curr->next;
                    }
                    helpy->next=curr->next;
                    curr->next=helpy;
                }
            
                i++;
                hpi=hpi->next;
                ph=ph->next;
            }
            break;
        
        }
        pi=pi->next;
    }
    print_double_list();
    //print_stack();
    return 1;
}


/**
 * @brief Vote
 *
 * @param pid The player's id
 * 
 * @param vote_evidence The vote's evidence
 *
 * @return 1 on success
 *         0 on failure
 */
int vote(int pid, int vote_evidence) {
    
    struct Players* pi=players_head;
    //for first player
    if(pi->pid==pid){
        pi->evidence=pi->evidence+vote_evidence;
    }else{
        pi=pi->next;
        while(pi!=players_sentinel){
            if(pi->pid==pid){
                pi->evidence=pi->evidence+vote_evidence;
            }
            pi=pi->next;
        }
    }
    //find the pid with the biggest pid and eject him
    int max=-1;
    int pidMax;
    
    if(max<pi->evidence){
        max=pi->evidence;
    }
    pi=pi->next;
    while(pi!=players_sentinel){
        if(max<pi->evidence){
            max=pi->evidence;
            pidMax=pi->pid;
        }
        pi=pi->next;
    }
    if(max>0){
        printf("the max evidence is %d and the player who has it is: %d\n",max,pidMax);
        ejectAndImport(pidMax,0,0);
        print_double_list();
    }
    return 1;
}


/**
 * @brief Give Away Work
 *
 * @return 1 on success
 *         0 on failure
 */
int give_work() {
    struct Players* pi=players_head;
    struct Players* minpi=NULL;
    struct Players* maxpi=NULL;
    int max=-1;
    int min=1000;
    while(pi!=players_sentinel){
        printf("THE PLAYER IS : <%d> ",pi->pid);
        if(min>lcount(pi)){
            min=lcount(pi);
            minpi=pi;
        }
        if(max<lcount(pi)){
            max=lcount(pi);
            maxpi=pi;
        }
        printf("\n");
        pi=pi->next;
    }
    printf("MIN PLAYER %d , min %d\n",minpi->pid,min);
    printf("MAX PLAYER %d , max %d\n",maxpi->pid,max);
    return 1;
}

//delete a node
void delete(struct Tasks* node, int tid){
    // Store head node 
    struct Tasks* temp = node, *prev; 
  
    // If head node itself holds the key to be deleted 
    if (temp != NULL && temp->tid == tid) 
    { 
        node = temp->next;   // Changed head 
        free(temp);               // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp->tid != tid) 
    { 
        prev = temp; 
        temp = temp->next; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) return; 
  
    // Unlink the node from linked list 
    prev->next = temp->next; 
  
    free(temp);
}

//function to find the player with the fewest tasks
int lcount(struct Players* node){
    printf("\t");
    int counter=0;
    struct Tasks* ti=node->tasks_head;
    while(ti!=node->tasks_sentinel){
        if(ti->tid==0){
            if(ti->next==node->tasks_sentinel)
                break;
            else
            {
                ti=ti->next;
            }
            
        }
        counter++;
        ti=ti->next;
    }
    //printf("\n");
    return counter;
}



/**
 * @brief Terminate
 *
 * @return 1 on success
 *         0 on failure
 */
int terminate() {
    struct Players* pi=players_head;
    int counterAlien=0;
    int counterCrewmate=0;
    int counterStack=0;
    if(pi->is_alien==1){
        counterAlien++;
    }else{
        counterCrewmate++;
    }
    pi=pi->next;
    while(pi!=players_sentinel){
        if(pi->is_alien==1){
            counterAlien++;
        }else{
            counterCrewmate++;
        }
        pi=pi->next;
    }
    int allin=tasks_head->tasks_count[0]+tasks_head->tasks_count[1]+tasks_head->tasks_count[2];
    struct Tasks* teli=tasks_stack->head;
    while(teli!=NULL){
        counterStack++;
        teli=teli->next;
    }
    //check who wins:
    if(counterAlien>=counterCrewmate){
        printf("Aliens win.\n");
    }else if(counterAlien==0 || allin==counterStack+1){
        printf("Crewmates win.\n");
    }
    return 1;
}

void addToStack(struct Tasks* this){

    struct  Tasks* forStack;
    //printf("Stack goes to <%d> and has difficulty <%d>\n",this->tid,this->difficulty);
    tasks_stack->count=tasks_stack->count+1;
    if(this->difficulty==1 ||this->difficulty==2 ||this->difficulty==3){
        if(tasks_stack->head==NULL){
            forStack=(struct  Tasks*)malloc(sizeof(struct  Tasks));
            forStack->tid=this->tid;
            forStack->difficulty=this->difficulty;
            forStack->next=NULL;
            tasks_stack->head=forStack;
        }else{
            forStack=(struct  Tasks*)malloc(sizeof(struct  Tasks));
            forStack->tid=this->tid;
            forStack->difficulty=this->difficulty;
            forStack->next=tasks_stack->head;
            tasks_stack->head=forStack;
        }
    }
}


void ejectAndImport(int pid,int pidi,int number){

    struct Players* pi, *before, *after, *thisPlayer, *theEjected;
    struct Tasks* thisHead, *ti, *min, *ti2;
    pi=players_head;
    int pidtofind;
    int wnumber;
    int found=0;

    if(pidi!=0){
        pidtofind=pidi;
        wnumber=number;
    }else{
        pidtofind=0;
    }
    //FIRST PART:DELETE THE PLAYER AND TAKE HIS TASKS.-------------
    //make prev point to the last element before head and after point;
    before=players_head->prev;
    after=players_head->next;
    //for first player
    if(pi->pid==pidtofind && pidtofind!=0){
        pi->evidence=wnumber;
    }
    if(pi->pid==pid){
        before->next=pi->next;
        after->prev=pi->prev;
        players_head=pi->next;
        theEjected=pi;
        thisHead=pi->tasks_head;
        found=1;
        free(pi);
        //print_players(players_head);    
    }else{
        //moving the pointers one place further
        pi=pi->next;
        before=before->next;
        after=after->next;
        while(pi!=players_sentinel){
            if(pi->pid==pidtofind && pidtofind!=0){
                pi->evidence=wnumber;
            }
            if(pi->pid==pid){
                before->next=pi->next;
                after->prev=pi->prev;
                theEjected=pi;
                thisHead=pi->tasks_head;
                found=1;
                free(pi);
                //print_players(players_head); 
                break;   
            }
            //moving the pointers one place further
            pi=pi->next;
            before=before->next;
            after=after->next;
        }
    }
    //There is a chance that the player that we are looking for has been deleted so:
    //printf("found??: %d\n",found);
    if(found==1){
    //SECOND PART:FIND PLAYER <pid_min>-------------------------
    int i=0;
    int minimum=10;
    pi=players_head;
    ti=pi->tasks_head;
    min=pi->tasks_head;
    thisPlayer=pi;

    //for first player
    if(pi->is_alien==0 && pi->tasks_head->difficulty!=0){
        ti=pi->tasks_head;
        while(ti!=pi->tasks_sentinel){
                if(ti->tid==-1 || ti->tid==0)
                    break;
                if(ti->difficulty!=0)
                    i++;
                ti=ti->next;
            }
        if(i>0){
            if(i<=minimum){
                minimum=i;
                min=ti;
                thisPlayer=pi;
            }
        }
        i=0;    
        //printf("\n");
        pi=pi->next;
    }else{
        pi=pi->next;
    }
    //for the rest of the players
    //pi=pi->next;
    while(pi!=players_sentinel){
        if(pi->is_alien==0 && pi->tasks_head->difficulty!=0){
            ti=pi->tasks_head;
            while(ti!=pi->tasks_sentinel){
                //printf("this is tid : <%d %d>\n",ti->tid,ti->difficulty);
                if(ti->tid==-1 || ti->tid==0)
                    break;
                if(ti->difficulty!=0)
                    i++;
                ti=ti->next;
            }
            printf("llull\n");
            if(i>0){
                if(i<=minimum){
                    minimum=i;
                    min=ti;
                    thisPlayer=pi;
                }
            }
            i=0;    
            pi=pi->next;
        }else{
            pi=pi->next;
        }
    }
    struct Tasks* ones, *fone=0, *lone;
    struct Tasks* twos, *ftwo=0, *ltwo;
    struct Tasks* threes, *fthree=0, *lthree;
    struct Tasks* point1, *point2, *point3;
    int check1=0;
    int check2=0;
    int check3=0;

    //printf("so this is the pid min : %d\n",thisPlayer->pid);
    //THIRD PART:IMPLEMENT THE TASKS INTO <pid mi> 
    //take the tasks of the ejected player divided at his difficulty
    while(thisHead!=theEjected->tasks_sentinel){
        //for difficulty==1 create a new list------------------------------------------------
        printf("this head tid <%d %d>\n",thisHead->tid,thisHead->difficulty);
        if(thisHead->difficulty==1){
            ones=(struct  Tasks*)malloc(sizeof(struct  Tasks));
            ones->tid=thisHead->tid;
            ones->difficulty=thisHead->difficulty;
            if(check1==0){
                point1=ones;
                fone=ones;
                check1=1;
            }else{
                point1->next=ones;
                point1=point1->next;
            }
            if(thisHead->next->difficulty!=1){
                point1->next=NULL;
                lone=ones;
                lone->next=NULL;
                ones->next=NULL;
            }
        }else if(thisHead->difficulty==2){//for difficulty==2 create a new list----------------
            twos=(struct  Tasks*)malloc(sizeof(struct  Tasks));
            twos->tid=thisHead->tid;
            twos->difficulty=thisHead->difficulty;
            if(check2==0){
                point2=twos;
                ftwo=twos;
                check2=1;
            }else{
                point2->next=twos;
                point2=point2->next;
            }
            if(thisHead->next->difficulty!=2){
                point2->next=NULL;
                ltwo=twos;
                ltwo->next=NULL;
                twos->next=NULL;
            }
        }else if(thisHead->difficulty==3){//for difficulty==3 create a new list---------------------------------------------
            threes=(struct  Tasks*)malloc(sizeof(struct  Tasks));
            threes->tid=thisHead->tid;
            threes->difficulty=thisHead->difficulty;
            if(check3==0){
                point3=threes;
                fthree=threes;
                check3=1;
            }else{
                point3->next=threes;
                point3=point3->next;
            }
            if(thisHead->next->difficulty!=2){
                point3->next=NULL;
                lthree=threes;
                lthree->next=NULL;
                threes->next=NULL;
            }
        }else{
            break;
        }
        thisHead=thisHead->next;
    }
    //insert the three lists into <pidmin> tasks
    int toBreak=0;
    check1=0;
    check2=0;
    check3=0;
    ti=thisPlayer->tasks_head;
    struct Tasks* curr;
    //check for first second and third list for head only
    if(fone!=NULL){
        if(thisPlayer->tasks_head==NULL || thisPlayer->tasks_head->difficulty>=1){
            lone->next=thisPlayer->tasks_head;
            thisPlayer->tasks_head=fone;
            if(thisPlayer->tasks_head->next==thisPlayer->tasks_sentinel){
                toBreak=1;
            }
            check1=0;
        }
    }
    if(ftwo!=NULL){
        if(thisPlayer->tasks_head==NULL || (thisPlayer->tasks_head->difficulty>=2 && thisPlayer->tasks_head->difficulty!=1)){
            ltwo->next=thisPlayer->tasks_head;
            thisPlayer->tasks_head=ftwo;
            if(thisPlayer->tasks_head->next==thisPlayer->tasks_sentinel){
                toBreak=1;
            }
            check2=0;
        }
    }
    int cu=0;
    if(toBreak==0){
        curr=thisPlayer->tasks_head;
            while(curr!=thisPlayer->tasks_sentinel){//insert after the first ti->difficulty=1--
                if(curr->tid==0){
                        break;
                }        
                if(ftwo!=NULL && ((curr->next==thisPlayer->tasks_sentinel ||curr->next->difficulty==3)&& fthree==NULL)&& check2==0){
                    ltwo->next=curr->next;
                    curr->next=ftwo;
                    check2=1;
                }
                if(curr->next==thisPlayer->tasks_sentinel && check3==0 && fthree!=NULL){
                    lthree->next=curr->next;
                    curr->next=fthree;
                    check3=1;
                }
                curr=curr->next;
                cu++;
            }
        }
        printf("loli\n");
        /*struct Tasks* del=thisPlayer->tasks_head;
        if(del->difficulty==1 || del->difficulty==2 || del->difficulty==3){
            while(del!=thisPlayer->tasks_sentinel){
                if(del->tid==del->next->tid){
                    delete(thisPlayer->tasks_head,del->tid);
                }
                if(del->next->tid==0 || del->next->tid==-1 || del->next==NULL)
                    break;
                del=del->next;    
            }
        }*/
    }
}


/**
 * @brief Print Players
 *
 * @return 1 on success
 *         0 on failure
 */
int print_players() {

    struct Players *p = players_head;

    //first player
    printf("Players= ");
    printf("<%d:%d> ",p->pid,p->is_alien);
    p = p->next;

    while(p != players_sentinel){
        printf("<%d:%d> ",p->pid,p->is_alien);
        p = p->next;
    }
    printf("\n");

    return 1;
}

/**
 * @brief Print Tasks
 *
 * @return 1 on success
 *         0 on failure
 */
int print_tasks() {
    struct Tasks *p=tasks_head->head;
    
    printf("Tasks= ");
    while(p!=NULL){
        printf("<%d,%d> ",p->tid,p->difficulty);
        p=p->next;
    }
    
    printf("\n");
    return 1;
}

/**
 * @brief Print Stack
 *
 * @return 1 on success
 *         0 on failure
 */

int print_stack() {
    printf("Stack_Tasks:\n");
    struct Tasks* teli=tasks_stack->head;
    while(teli!=NULL){
        printf("<%d %d>\n",teli->tid,teli->difficulty);
        teli=teli->next;
    }

    return 1;
}

/**
 * @brief Print Players & Task List
 *
 * @return 1 on success
 *         0 on failure
 */
int print_double_list() {
    struct Tasks* ti;
    struct Players* pi;
    int i=2;

    pi = players_head;
    if(pi->is_alien==1){
        pi = pi->next;
    }else{
        printf("Player 1 with pid %d has tasks: ",pi->pid);
        ti = pi->tasks_head;
        while (ti != NULL){
            if (ti->difficulty == 1 || ti->difficulty==2 || ti->difficulty==3)
                printf(" <%d,%d> ",ti->tid, ti->difficulty);
            ti = ti->next;
        }
        printf("\n");
        pi=pi->next;
    }
    while(pi != players_sentinel){
        if(pi->is_alien==1){
        pi = pi->next;
        }else{
        printf("Player %d with pid %d has tasks: ",i,pi->pid);
        i++;
        ti = pi->tasks_head;
        while (ti != NULL){
            if (ti->tid != -1 && ti->difficulty!=0)
                printf(" <%d,%d> ",ti->tid, ti->difficulty);
            ti = ti->next;
        }
        printf("\n");
        pi=pi->next;
        }
    }
    return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */

int free_all(void) {
    return 1;
}
