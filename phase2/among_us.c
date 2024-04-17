/***************************************************
 *                                                 *
 * file: among_us.c                                *
 *                                                 *
 * @Author  Skerdi Basha                           *
 * @Version 1-12-2020                              *
 * @email   sbash@csd.uoc.gr                       *
 *                                                 *
 * @brief   Implementation of among_us.h           *
 *                                                 *
 ***************************************************
 */

#include "among_us.h"

//player pointer
struct Player* players_sentinel;

//for event T
struct HT_Task** tasks;
//hash table
    //hash function
    int a;
    int b;
    int k;
    int m;
    
//priority queue
struct HT_Task* priority_queue;
int size = 0; 

//for event E
struct Player* last;
struct Task* carrier;
int current=0;
//dumbies
int play=0;
int eject=0;


//for event S
struct Player* prev;



unsigned int primes_g[650] = { 
                                 179,    181,    191,    193,    197,    199,    211,    223,    227,    229, 
                                 233,    239,    241,    251,    257,    263,    269,    271,    277,    281, 
                                 283,    293,    307,    311,    313,    317,    331,    337,    347,    349, 
                                 353,    359,    367,    373,    379,    383,    389,    397,    401,    409, 
                                 419,    421,    431,    433,    439,    443,    449,    457,    461,    463, 
                                 467,    479,    487,    491,    499,    503,    509,    521,    523,    541, 
                                 547,    557,    563,    569,    571,    577,    587,    593,    599,    601, 
                                 607,    613,    617,    619,    631,    641,    643,    647,    653,    659, 
                                 661,    673,    677,    683,    691,    701,    709,    719,    727,    733, 
                                 739,    743,    751,    757,    761,    769,    773,    787,    797,    809, 
                                 811,    821,    823,    827,    829,    839,    853,    857,    859,    863, 
                                 877,    881,    883,    887,    907,    911,    919,    929,    937,    941, 
                                 947,    953,    967,    971,    977,    983,    991,    997,   1009,   1013, 
                                1019,   1021,   1031,   1033,   1039,   1049,   1051,   1061,   1063,   1069, 
                                1087,   1091,   1093,   1097,   1103,   1109,   1117,   1123,   1129,   1151, 
                                1153,   1163,   1171,   1181,   1187,   1193,   1201,   1213,   1217,   1223, 
                                1229,   1231,   1237,   1249,   1259,   1277,   1279,   1283,   1289,   1291, 
                                1297,   1301,   1303,   1307,   1319,   1321,   1327,   1361,   1367,   1373, 
                                1381,   1399,   1409,   1423,   1427,   1429,   1433,   1439,   1447,   1451, 
                                1453,   1459,   1471,   1481,   1483,   1487,   1489,   1493,   1499,   1511, 
                                1523,   1531,   1543,   1549,   1553,   1559,   1567,   1571,   1579,   1583, 
                                1597,   1601,   1607,   1609,   1613,   1619,   1621,   1627,   1637,   1657, 
                                1663,   1667,   1669,   1693,   1697,   1699,   1709,   1721,   1723,   1733, 
                                1741,   1747,   1753,   1759,   1777,   1783,   1787,   1789,   1801,   1811, 
                                1823,   1831,   1847,   1861,   1867,   1871,   1873,   1877,   1879,   1889, 
                                1901,   1907,   1913,   1931,   1933,   1949,   1951,   1973,   1979,   1987, 
                                1993,   1997,   1999,   2003,   2011,   2017,   2027,   2029,   2039,   2053, 
                                2063,   2069,   2081,   2083,   2087,   2089,   2099,   2111,   2113,   2129, 
                                2131,   2137,   2141,   2143,   2153,   2161,   2179,   2203,   2207,   2213, 
                                2221,   2237,   2239,   2243,   2251,   2267,   2269,   2273,   2281,   2287, 
                                2293,   2297,   2309,   2311,   2333,   2339,   2341,   2347,   2351,   2357, 
                                2371,   2377,   2381,   2383,   2389,   2393,   2399,   2411,   2417,   2423, 
                                2437,   2441,   2447,   2459,   2467,   2473,   2477,   2503,   2521,   2531, 
                                2539,   2543,   2549,   2551,   2557,   2579,   2591,   2593,   2609,   2617, 
                                2621,   2633,   2647,   2657,   2659,   2663,   2671,   2677,   2683,   2687, 
                                2689,   2693,   2699,   2707,   2711,   2713,   2719,   2729,   2731,   2741, 
                                2749,   2753,   2767,   2777,   2789,   2791,   2797,   2801,   2803,   2819, 
                                2833,   2837,   2843,   2851,   2857,   2861,   2879,   2887,   2897,   2903, 
                                2909,   2917,   2927,   2939,   2953,   2957,   2963,   2969,   2971,   2999, 
                                3001,   3011,   3019,   3023,   3037,   3041,   3049,   3061,   3067,   3079, 
                                3083,   3089,   3109,   3119,   3121,   3137,   3163,   3167,   3169,   3181, 
                                3187,   3191,   3203,   3209,   3217,   3221,   3229,   3251,   3253,   3257, 
                                3259,   3271,   3299,   3301,   3307,   3313,   3319,   3323,   3329,   3331, 
                                3343,   3347,   3359,   3361,   3371,   3373,   3389,   3391,   3407,   3413, 
                                3433,   3449,   3457,   3461,   3463,   3467,   3469,   3491,   3499,   3511, 
                                3517,   3527,   3529,   3533,   3539,   3541,   3547,   3557,   3559,   3571, 
                                3581,   3583,   3593,   3607,   3613,   3617,   3623,   3631,   3637,   3643, 
                                3659,   3671,   3673,   3677,   3691,   3697,   3701,   3709,   3719,   3727, 
                                3733,   3739,   3761,   3767,   3769,   3779,   3793,   3797,   3803,   3821, 
                                3823,   3833,   3847,   3851,   3853,   3863,   3877,   3881,   3889,   3907, 
                                3911,   3917,   3919,   3923,   3929,   3931,   3943,   3947,   3967,   3989, 
                                4001,   4003,   4007,   4013,   4019,   4021,   4027,   4049,   4051,   4057, 
                                4073,   4079,   4091,   4093,   4099,   4111,   4127,   4129,   4133,   4139, 
                                4153,   4157,   4159,   4177,   4201,   4211,   4217,   4219,   4229,   4231, 
                                4241,   4243,   4253,   4259,   4261,   4271,   4273,   4283,   4289,   4297, 
                                4327,   4337,   4339,   4349,   4357,   4363,   4373,   4391,   4397,   4409, 
                                4421,   4423,   4441,   4447,   4451,   4457,   4463,   4481,   4483,   4493, 
                                4507,   4513,   4517,   4519,   4523,   4547,   4549,   4561,   4567,   4583, 
                                4591,   4597,   4603,   4621,   4637,   4639,   4643,   4649,   4651,   4657, 
                                4663,   4673,   4679,   4691,   4703,   4721,   4723,   4729,   4733,   4751, 
                                4759,   4783,   4787,   4789,   4793,   4799,   4801,   4813,   4817,   4831, 
                                4861,   4871,   4877,   4889,   4903,   4909,   4919,   4931,   4933,   4937, 
                                4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003, 
                                5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087, 
                                5099,   5101,   5107,   5113,   5119,   5147,   5153,   5167,   5171,   5179, 
                            };

/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize() {
    //for event P
        players_tree=NULL;
        players_sentinel=NULL;

    //for event T
        //initialize counter
        general_tasks_ht.count=0;
        //initialize general.tasks and make tasks equal to it so its more easier to handle.
        general_tasks_ht.tasks=malloc(sizeof(struct HT_Task*)*max_tasks_g);
        tasks=general_tasks_ht.tasks;

    //for hash function
    int bounds=0;
    //find the element after max tid
    for(int i=0; i<650; i++){
        if(primes_g[i]>max_tid_g){
            bounds=primes_g[i];
            m=bounds;
            break;
        }
    }
    //initialize a and b
    a=(rand()%((bounds-1)-1+1)+1);
    b=(rand()%((bounds-1)-0+1)+0);


    //for event I
        completed_tasks_pq=(struct Completed_Tasks_PQ*)malloc(sizeof(struct Completed_Tasks_PQ));
        completed_tasks_pq->size=0;
        //make priority queue array equal to completed_tasks->tasks so its more easier to handle
        completed_tasks_pq->tasks= (struct HT_Task*)malloc(sizeof(struct HT_Task)*max_tasks_g);
        priority_queue=completed_tasks_pq->tasks;
        
    //for event E
        //last is a temp Player to carry the player that will be deleted
        last=(struct Player*)malloc(sizeof(struct Player));
        last->pid=0;
        //also carrier contains the tasks that will be inserted to pid_2
        carrier=NULL;

    //for event S
        //to hande predeseccor
        prev=NULL;

    return 1;
}

/**
 * @brief Register Player
 *
 * @param pid The player's id
 *
 * @param is_alien The variable that decides if he is an alien or not
 * @return 1 on success
 *         0 on failure
 */
int register_player(int pid, int is_alien) {
    struct Player* temp= (struct Player*)malloc(sizeof(struct Player));
    struct Player* new_player = (struct Player*)malloc(sizeof(struct Player));

    //create the player's characteristics
    new_player->pid=pid;
    new_player->is_alien=is_alien;
    new_player->evidence=0;
    new_player->tasks=NULL;

    //connect him with the others
    if(players_tree==NULL){
        //create the sentinel
        temp=(struct Player*)malloc(sizeof(struct Player));
        temp->pid=-1;
        temp->is_alien=0;
        temp->evidence=0;
        temp->tasks=NULL;
        temp->parrent=temp->lc=temp->rc=NULL;
        players_sentinel=temp;

        //make head=new_player
        players_tree=new_player;
        players_tree->parrent=NULL;
        players_tree=insert_player(players_tree,new_player);
        players_tree->lc=players_tree->rc=players_sentinel;
    }else{
        //add the new player
        insert_player(players_tree,new_player);
        new_player->lc=new_player->rc=players_sentinel;
    }
    play++;
    print_players();
    
    return 1;
}



/**
 * @brief Insert Task in the general task hash table
 *
 * @param tid The task id
 * 
 * @param difficulty The difficulty of the task
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_task(int tid, int difficulty) {
    //create a temporary node and insert the values
    struct HT_Task* temp=(struct HT_Task*)malloc(sizeof(struct HT_Task));
    temp->tid=tid;
    temp->difficulty=difficulty;
    
    //the position where the task will be inserted
    int index=hash(tid);
    
    //insert the task into the hash table
    if(tasks[index]==NULL){
        temp->next=NULL;
        tasks[index]=temp;
    }else{
        temp->next= tasks[index];
        tasks[index]=temp;
    }

    //also increase counter by one
    general_tasks_ht.count++;
    print_tasks();
    
    return 1;
}


/**
 * @brief Distribute Tasks to the players
 * @return 1 on success
 *         0 on failure
 */
int distribute_tasks() {    
    //my counters
    int seeing=0;   //it counts if O(n) is good
    int cnt=0;      //it counts if cnt is greater or less than general.count 
    int pos=0;      //it holds the position of the hash table

    //to check if a player is the maximum player
    int check=0;

    //create a similiar hash table in order to fill the players tasks
    struct HT_Task** ti;
    struct HT_Task* head;
    ti=tasks;
    
    //pi is the minimum player
    struct Player* pi=minPlayer(players_tree);
    
    //while all the tasks has been successfully shared
    while(cnt<general_tasks_ht.count){
        if(pos==max_tasks_g){
            pos=0;
        }
        if(ti[pos]==NULL){
            pos++;
        }
        if(ti[pos]!=NULL){
            head=ti[pos];
            //O(n) is good because i move the counter only if ther is a task (check seeing)
            while(head!=NULL){
                //check for aliens
                if(pi->is_alien!=0){
                //check if player is the last so to go back to start
                    if(pi->pid==maxPlayer(players_tree)->pid){
                        pi=minPlayer(pi);
                    }else{
                        //if next again is alien
                        pi=nextOf(pi);
                        if(pi->is_alien!=0)
                            pi=nextOf(pi);
                    }
                }
                //go to crewmates
                if(pi->pid==maxPlayer(players_tree)->pid)
                    //make check equals to 1 so to go back to minimum after insertion
                    check=1;
                //we keep an eye on O(n)
                seeing++;
                //insert the task at the task tree
                pi->tasks=insert_tree_task(pi->tasks,head->tid,head->difficulty);
                //revalidate lcnts
                lcounter(pi->tasks);

                //go to next task at the same chain and increase cnt
                head=head->next;
                cnt++;

                if(check==1){
                    pi=minPlayer(players_tree);
                    check=0;
                }else{
                    //if next player is max and also he is an alien make it immediatly minPlayer
                    if(nextOf(pi)->pid==maxPlayer(players_tree)->pid && nextOf(pi)->is_alien!=0){
                        pi=nextOf(minPlayer(players_tree));     
                    }
                    pi=nextOf(pi);
                }
            }
            pos++;
        }
    }
    //printf("Seeing is: %d = max_tasks so O(n) is good\n",seeing);
    print_double_tree();

    return 1;
}



/**
 * @brief Implement Task
 *
 * @param pid The player's id
 *
 * @param tid The task's tid
 *
 * @return 1 on success
 *         0 on failure
 */
int implement_task(int pid, int tid) {
    
    //temp pointers
    struct Player* pi;
    struct Task* ti;

    pi=search_player(players_tree,pid); //search the player with this pid if he exists
    if(pi!=NULL){
        ti=search_task(pi->tasks,tid);
        if(ti!=NULL){ //if that tid exists add it to priority queue and delete it from tasks
            struct HT_Task temp;
            temp.tid=ti->tid;
            temp.difficulty=ti->difficulty;
            temp.next=NULL;
            insert_task_pq(temp);
            completed_tasks_pq->size++;
        }
        pi->tasks=deleteANode(pi->tasks,tid);
        lcounter(pi->tasks); //revalidate lcounts
    }

    print_double_tree();
  
    return 1;
}


/**
 * @brief Eject Player
 * 
 * @param pid_1 The ejected player's id
 *
 * @param pid_2 The crewmates id
 *
 * @return 1 on success
 *         0 on failure
 */
int eject_player(int pid_1, int pid_2) {
    eject++;
    struct Player* pi=players_tree;
    
    //delete the player
    delete_player(pi,pid_1);
    if(last->pid!=0){
        players_tree->pid=last->pid;
        players_tree->tasks=last->tasks;
        players_tree->is_alien=last->is_alien;
        last->pid=0;
        current=0;
    }
    
    //if carrier is null it means that plauyer->tasks are null
    if(carrier!=NULL){
        insert_pid2(carrier,players_tree,pid_2);
        carrier=NULL;
    }
    print_double_tree();
    return 1;
}


/**
 * @brief Witness Eject Player
 *
 * @param pid_1 The ejected player's id
 * 
 * @param pid_2 The crewmate's pid
 *
 * @param pid_a The alien's pid
 * 
 * @param number_of_witnesses The number of witnesses
 *
 * @return 1 on success
 *         0 on failure
 */
int witness_eject(int pid_1, int pid_2, int pid_a, int number_of_witnesses){
    eject++;
    struct Player* pi=players_tree;
    
    //delete the player
    delete_player(pi,pid_1);
    if(last->pid!=0){
        players_tree->pid=last->pid;
        players_tree->tasks=last->tasks;
        players_tree->is_alien=last->is_alien;
        last->pid=0;
        current=0;
    }
    
    //if carrier is null it means that plauyer->tasks are null
    if(carrier!=NULL){
        insert_pid2(carrier,players_tree,pid_2);
        carrier=NULL;
    }
    search_insert_number(players_tree,pid_a,number_of_witnesses);
    print_players_evidence(players_tree);
    return 1;
}


/**
 * @brief Sabotage
 *
 * @param number_of_tasks The number of tasks to be sabotaged
 *
 * @param pid The player's id
 * 
 * @return 1 on success
 *         0 on failure
 */
int sabotage(int number_of_tasks, int pid) {
    int i=0;
    int j=0;
    int check =0;
    int return_to_min=0;
    struct Player* him=search_player(players_tree,pid); //make him player with that pid
    
    //BASIC WHILE
    while(i<number_of_tasks){
        //FIND THE PLAYER THAT IS FLOOR(NUMBER_IF_TASKS/2) POSITIONS BEFORE HIM
        while(j<number_of_tasks/2 && check==0){
            //if the player is the last in the tree
            if(him==maxPlayer(players_tree)){
                him=minPlayer(players_tree);
                j++;
                if(j>=number_of_tasks/2)
                    break;
            }
            //if the player is alien
            if(him->is_alien!=0){
                him=nextOf(him);
                j++;
                if(j>=number_of_tasks/2)
                    break;
                //if there is alien next to alien
                if(him->is_alien!=0){
                    him=nextOf(him);
                    j++;
                    if(j>=number_of_tasks/2)
                        break;
                }
            }
            him=nextOf(him);
            j++;
        }
        //if him->is alien!=0 call predecessor
        while(him->is_alien!=0 && check==0){
            if(him=minPlayer(players_tree)){
                him=maxPlayer(players_tree);
                if(him->is_alien!=0)
                    break;
            }
            him=prevOf(him);
        }
        //make check 1 so dont go again to loop
        check=1;


        //NOW WE MADE ALL THE CHECK UPS WE ARE READY TO SHARE THE TASKS
        if(him->pid==maxPlayer(players_tree)->pid){
            return_to_min=1;
        }
        //if there are aliens 
        while(him->is_alien!=0){
            if(him->pid==maxPlayer(players_tree)->pid){
                him=minPlayer(players_tree);
                return_to_min=0;
            }else{
                him=nextOf(him);
            }
        }
        //for crewmates
        if(priority_queue[i].tid!=0){
            him->tasks=insert_tree_task(him->tasks,priority_queue[i].tid,priority_queue[i].difficulty);
            lcounter(him->tasks);
        }
        completed_tasks_pq->size--;
        delete();
        if(return_to_min==1){
            him=minPlayer(players_tree);
            while(him->is_alien!=0){
                him=nextOf(him);
            }
            return_to_min=0;
        }else{
            him=nextOf(him);
        }
        i++;
    }
    print_double_tree();
    
    return 1;
}

/**
 * @brief Vote
 *
 * @param pid_1 The suspicious player's id
 *
 * @param pid_2 The crewmate's pid
 * 
 * @param vote_evidence The vote's evidence
 *
 * @return 1 on success
 *         0 on failure
 */
int vote(int pid_1, int pid_2, int vote_evidence) {
    eject++;
    search_insert_number(players_tree,pid_1,vote_evidence);
    
    //i will do it manually
    struct Player* pi=minPlayer(players_tree);
    struct Player* maxpi;
    int max=-1;
    while(pi!=players_sentinel){
        //revalidate max
        if(max<pi->evidence){
            max=pi->evidence;
            maxpi=pi;
        }
        if(pi==maxPlayer(players_tree))
            break;
        pi=nextOf(pi);
    }
    printf("this is the biggest evidence: %d and this is the biggest pid: %d\n",max,maxpi->pid);
    //delete the player with max evidence
    delete_player(players_tree,maxpi->pid);
    if(last->pid!=0){
        players_tree->pid=last->pid;
        players_tree->tasks=last->tasks;
        players_tree->is_alien=last->is_alien;
        last->pid=0;
        current=0;
    }
    
    //if carrier is null it means that plauyer->tasks are null
    if(carrier!=NULL){
        insert_pid2(carrier,players_tree,pid_2);
        carrier=NULL;
    }
    print_players_evidence(players_tree);
    return 1;
}

/**
 * @brief Give Away Work
 *
 * @param pid_1 The existing crewmate's id
 *
 * @param pid_2 The new crewmate's pid
 *
 * @return 1 on success
 *         0 on failure
 */
int give_work(int pid_1, int pid_2) {
    play++;
    struct Player* new_player = (struct Player*)malloc(sizeof(struct Player));

    //create the player's characteristics
    new_player->pid=pid_2;
    new_player->is_alien=0;
    new_player->evidence=0;
    new_player->tasks=NULL;
    //insert him at the tree
    if(players_tree!=NULL){
        insert_player(players_tree,new_player);
        new_player->lc=new_player->rc=players_sentinel;
    }
    //save the 2 players and pass them to the function
    struct Player* pi1=search_player(players_tree,pid_1);
    struct Player* pi2=search_player(players_tree,pid_2);
    split(pi1,pi2);
    print_double_tree();
    

    return 1;
}

/**
 * @brief Terminate
 *
 * @return 1 on success
 *         0 on failure
 */
int terminate() {
    int crewmates=0;
    int aliens=0;
    struct Player* pi=minPlayer(players_tree);

    while(pi!=players_sentinel){
        if(pi->is_alien==0)
            crewmates++;
        else
            aliens++;
        if(pi->pid==maxPlayer(players_tree)->pid)
            break;
        pi=nextOf(pi);    
    }

    if(aliens>crewmates){
        printf("Aliens win.\n");
    }else if(aliens==0 || max_tasks_g>=completed_tasks_pq->size){
        printf("Crewmates win.\n");
    }
    return 1;
}

/**
 * @brief Print Players
 *
 * @return 1 on success
 *         0 on failure
 */
int print_players() {
    struct Player* pi=players_tree;

    printf("Players= ");
    inorder(pi);
    printf("\n");
}



/**
 * @brief Print Tasks
 *
 * @return 1 on success
 *         0 on failure
 */
int print_tasks() {
    //print them
    struct HT_Task* taski;
    for(int i=0; i<max_tasks_g; i++){
        taski=general_tasks_ht.tasks[i];
        printf("Chain %d: ",i);
        if(taski!=NULL){
            while(taski!=NULL){
                printf("<%d  %d>, ",taski->tid,taski->difficulty);
                taski=taski->next;
            }
            printf("\n");
        }else{
            printf("NULL\n");
        }
    }
    return 1;
}


/**
 * @brief Print Priority Queue
 *
 * @return 1 on success
 *         0 on failure
 */
int print_pq() {

    printf("Priority queue has this elements:\n");
    for(int i=0; i<size; i++){
        printf("<%d %d>\n",priority_queue[i].tid,priority_queue[i].difficulty);
    }
    if(completed_tasks_pq->size<0)
        completed_tasks_pq->size=0;
    return 1;
}

/**
 * @brief Print Players & Task tree
 *
 * @return 1 on success
 *         0 on failure
 */
int print_double_tree() {

    inorder_player_task(players_tree);

    return 1;
}


void inorder_task(struct Task* root) 
{ 
    if (root!=NULL) { 
        inorder_task(root->lc);
        printf(" <%d : %d>  ", root->tid, root->difficulty); 
        inorder_task(root->rc); 
    } 
}

void inorder_player_task(struct Player* proot) 
{ 
    if (proot!=players_sentinel) { 
        inorder_player_task(proot->lc);
        printf("Player %d =",proot->pid);
        struct Task* troot=proot->tasks;
        inorder_task(troot); 
        printf("\n");
        inorder_player_task(proot->rc); 
    } 
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

//HERE IS MY FUNCTIONS:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//FOR EVENT P------------------------------------------------------------------------------------------------

    //function that inserts a player at the "BST Player"
    struct Player* insert_player(struct Player* node, struct Player* new) 
    { 
        // If the tree is only the sentinel, return a new node */
        if (node == players_sentinel){ 
            return new;
        }
        // Otherwise, recur down the tree 
        if (new->pid < node->pid){
            //implement the parrent of the new node
            if(node->rc==players_sentinel || node->lc==players_sentinel){
                new->parrent=node;
            }
            node->lc = insert_player(node->lc, new); 
        }else if (new->pid > node->pid){ 
            //insert the parrent of the new node
            if(node->rc==players_sentinel || node->lc==players_sentinel){
                new->parrent=node;
            }
            node->rc = insert_player(node->rc, new); 
        }
        // return the (unchanged) node pointer 
        return node; 
    }

    //function to do inorder traversal of "BST Player" and print its pid and is_alien 
    void inorder(struct Player* root) 
    { 
        if (root!=players_sentinel) { 
            inorder(root->lc);
            printf("<%d : %d>  ", root->pid, root->is_alien); 
            inorder(root->rc); 
        } 
    } 

//FOR EVENT T------------------------------------------------------------------------------------------
    //A function that returns the position that tid should be inserted at hash table
    int hash(int tid){
     
        
        return(((a*tid+b)%m)%max_tasks_g);
    }

//FOR EVENT D------------------------------------------------------------------------------------------
    //a function to find the minimum player of the tree
    struct Player* minPlayer(struct Player* node){

        //make a pointer to the given node
        struct Player* curr=node;
        //access the tree until find the leftest child
        while(curr && curr->lc!=players_sentinel){
            curr=curr->lc;
        }
        return curr;
    }

    //a function to find the maximum player of the tree
    struct Player* maxPlayer(struct Player* node){

        //make a pointer to the given node
        struct Player* curr=node;
        while(curr->rc!=players_sentinel){
            curr=curr->rc;
        }
        return curr;
    }

    struct Player* nextOf(struct Player* n){ 
        // if next of this node has an element return this
        if (n->rc != players_sentinel) 
            return minPlayer(n->rc); 
  
        // else return the parrent of this node
        struct Player* p = n->parrent; 
        while (p != players_sentinel && n == p->rc) { 
            n = p; 
            p = p->parrent; 
        } 
        return p; 
    } 


    //function that inserts a task in the the tree task of one player
    struct Task* insert_tree_task(struct Task* node, int tid ,int dif) { 
        // If the tree is only the sentinel, return a new node */
        if (node == NULL){ 
            //print_double_tree();
            return newTask(tid,dif);
        }
        // Otherwise, recur down the tree 
        if (tid < node->tid){
            node->lc = insert_tree_task(node->lc, tid, dif); 
        }else if (tid > node->tid){ 
            node->rc = insert_tree_task(node->rc, tid, dif); 
        }
        if(tid==node->tid)
            return node;
        // return the (unchanged) node pointer 
        return node;
    }


    // function to create a new BST node
    struct Task* newTask(int tid, int dif){
        struct Task* temp=(struct Task*)malloc(sizeof(struct Task));
        temp->tid = tid;
        temp->difficulty=dif;
        temp->lcnt=0;
        temp->lc = temp->rc = NULL;
        return temp;
    }

    //function to validate lcnt
    void lcounter(struct Task* root){
        if (root!=NULL) { 
            lcounter(root->lc);
            //If node is a leaf lcnt=0
            if(root->lc==NULL && root->rc==NULL){
                root->lcnt=0;
            }else if(root->lc==NULL){ //if left of the nodeis null lcounter is null
                root->lcnt=0;    
            }else if(root->lc!=NULL){ //if left f the node isn't null validate lcouncter
                root->lcnt=1+root->lc->lcnt;
            }
            lcounter(root->rc); 
        } 
    }


//FOR EVENT I--------------------------------------------------------------------------------------------
    //function to search a given key 
    struct Player* search_player(struct Player* root, int pid){ 
        // if root is null print error and return root or if he exists return root 
        if (root == NULL || root->pid == pid){ 
            if(root==NULL){
                printf("---ERROR:Player didn't found!\n");
            }
            return root; 
        }
        //if pid > root->pid search right subtree
        if (root->pid < pid) 
            return search_player(root->rc, pid); 
  
        //if pid < root->pid search left subtree
        return search_player(root->lc, pid); 
    }

    //Function to delete a node from "BST Tasks" iterative
    struct Task* deleteANode(struct Task *root, int pid) {
        struct Task* curr = root; 
        struct Task* prev = NULL; 
  
        //recursivelly find pid or find NULL. 
        while (curr != NULL && curr->tid != pid) { 
            prev = curr; 
            if (pid < curr->tid) 
                curr = curr->lc; 
            else
                curr = curr->rc; 
        } 
  
        if (curr == NULL) { //if it is null we didnt found the node
            return root; 
        } 
  
        // Check if the task to be deleted has atmost one child. 
        if (curr->lc == NULL || curr->rc == NULL) { 
  
            // newCurr will replace the task to be deleted. 
            struct Task* newCurr; 
  
            // if the left child does not exist. 
            if (curr->lc == NULL) 
                newCurr = curr->rc; 
            else
                newCurr = curr->lc; 
  
            // check if the task to be deleted is the root. 
            if (prev == NULL) 
                return newCurr; 
  
            // check if the task to be deleted is prev's left or right child and then replace this with newCurr 
            if (curr == prev->lc) 
                prev->lc = newCurr; 
            else
                prev->rc = newCurr; 
  
            // free memory of the task to be deleted. 
            free(curr); 
        } 
  
        // task to be deleted has two children. 
        else { 
            struct Task* p = NULL; 
            struct Task* temp; 
  
            // Compute the inorder successor 
            temp = curr->rc; 
            while (temp->lc != NULL) { 
                p = temp; 
                temp = temp->lc; 
            } 
  
            // check if the parent of the inorder successor is the root or not 
            if (p != NULL) 
                p->lc = temp->rc; 
  
            // if the inorder successor was the root
            else
                curr->rc = temp->rc; 
  
            curr->tid = temp->tid; 
            curr->difficulty=temp->difficulty;
            free(temp); 
        } 
        return root; 
    } 


    //function to search a task at the "BST Task" tree
    struct Task* search_task(struct Task* root, int key){
        if(root==NULL){//print error and return root
            printf("Tid didn't found!\n");
            return root;
        }else if (root->tid==key){//print message and return root
            printf("Key found!\n");
            return root;
        }
        //search recursivelly the tree
        if(root->tid<key){
            return search_task(root->rc,key);
        }else if(root->tid>key){
            return search_task(root->lc,key);
        } 
    }

//FOR PRIORITY QUEUE----------------------------------------------------------------------------------------
    //INSERT
    void insert_task_pq(struct HT_Task task){ 
            //size = size + 1; 
            priority_queue[size] = task; 
            //call move up so as to maintain the priority
            size = size + 1; 
            sort(priority_queue);
        
    }

    //SORT THE ARRAY
    void sort(struct HT_Task *arr){
        int i, j; 
        //a humble bublesort
        for (i = 0; i < size-1; i++)           
            for (j = 0; j < size-i-1; j++)  
                if (arr[j].difficulty < arr[j+1].difficulty) 
                    swap(&arr[j], &arr[j+1]); 
    }

    //SWAP 2 NUMBERS
    void swap(struct HT_Task *p,struct HT_Task *q){
        struct HT_Task tmp;
        tmp = *p; 
        *p=*q;    
        *q=tmp;
    }

    //DELETE THE FIRST ELEMENT
    void delete(){
        struct HT_Task* temp;
        temp=priority_queue;

        if(size>=0){
            size=size-1;
            for(int i=0; i<size; i++){
                priority_queue[i]=temp[i+1];
            }
        }
    }


//FOR EVENT E-------------------------------------------------------------------------------------------------------
    
    //function to insert the tasks that holds the carrier into pid_2
    struct Player* insert_pid2(struct Task* carrier, struct Player* root,int pid){
        if (root == players_sentinel) //we didnt found player
            return root;

        if (pid < root->pid){ //search left subtree
        
            root->lc = insert_pid2(carrier,root->lc, pid);
 
        }else if (pid > root->pid){ //search right subtree
        
            root->rc = insert_pid2(carrier,root->rc, pid);
 
        }else { //add carrier to players->tasks
            root->tasks=change_tasks(carrier,root->tasks);
        }
    }



    struct Task* change_tasks(struct Task* curr,struct Task* this){
        struct Task* ti=minTask(carrier);
        while(ti!=NULL){//until carrier ends insert his tasks into players->tasks
            this=insert_tree_task(this,ti->tid,ti->difficulty);
            lcounter(this);
            if(ti->tid==maxTask(carrier)->tid)
                break;
            ti=nextTask(carrier,ti);
        }
        return this;
    }

    //min of task tree
    struct Task* minTask(struct Task* node){
        struct Task* curr=node;
        //access the tree until find the leftest child
        while(curr && curr->lc!=NULL){
            curr=curr->lc;
        }
        return curr;
    }

    //max of task tree
    struct Task* maxTask(struct Task* node){
        struct Task* curr=node;
        //access the tree until find the leftest child
        while(curr->rc!=NULL){
            curr=curr->rc;
        }
        return curr;
    }

    //next of task tree
    struct Task* nextTask(struct Task* root, struct Task* n) {  
        if (n->rc != NULL) 
            return minTask(n->rc); 
  
        struct Task* succ = NULL; 
  
        // Start from root and search for successor down the tree 
        while (root != NULL) { 
            if (n->tid < root->tid)  { 
                succ = root; 
                root = root->lc; 
            } 
            else if (n->tid > root->tid) 
                root = root->rc; 
            else
                break; 
        } 
        return succ; 
    } 
        


    struct Player* delete_player(struct Player* root, int pid){
        if (root == players_sentinel) //we didnt found the player
            return root;
 
        if (pid < root->pid){ //search left subtree
            root->lc = delete_player(root->lc, pid);
        }else if (pid > root->pid){ //search right subtree
            root->rc = delete_player(root->rc, pid);
        }else { //we found him
            carrier=root->tasks; //make carrier hold his tasks
    
            if (root->lc == players_sentinel) { //player with only one child or no child (left)
            
                struct Player* temp = root->rc;
                if(root->parrent!=NULL) //connect the parents
                    temp->parrent=root->parrent;
                if(players_tree->pid==pid) //if it is the root
                    players_tree=temp;
                    players_tree->tasks=temp->tasks;
                free(root);
        
                return temp;
            }else if (root->rc == players_sentinel) { //player with only one child or no child (right)
            
                struct Player* temp = root->lc;
                if(root->parrent!=NULL) //connect the parents
                    temp->parrent=root->parrent;
                if(players_tree->pid==pid) //if it is the root
                    players_tree=temp;
                free(root);

                return temp;
            }
            struct Player* temp=(struct Player*)malloc(sizeof(struct Player));

                //Compute the inorder successor 

                //temp holds the values of minPlayer(root->rc) 
                temp->pid=minPlayer(root->rc)->pid;
                temp->is_alien=minPlayer(root->rc)->is_alien;
                temp->tasks=minPlayer(root->rc)->tasks;
                temp->evidence=0;

                if(pid==players_tree->pid && current==0){ //if it is the root with 2 childs make last hold temp values
                    last->pid=temp->pid;
                    last->is_alien=temp->is_alien;
                    last->tasks=temp->tasks;

                    current=1;
                }else{ //else revalidate node with temp values
                    root->pid=temp->pid;
                    root->is_alien=temp->is_alien;
                    root->tasks=temp->tasks;
                }

                if(current!=1){ //finally delete minPlayer
                    root->rc=delete_player(root->rc,temp->pid);
                    return root;
                }else{
                    minPlayer(root->rc)->parrent->lc=players_sentinel;
                }
        }   
        return root;
    }

//FOR EVENT W-------------------------------------------------------------------------------------------------------------------
    //function that inserts number of witnesses at pid 
    struct Player* search_insert_number(struct Player* root, int pid, int number_of_witnesses) 
    { 
        // Base Cases: root is null or key is present at root 
        if (root == NULL || root->pid == pid){ //if we found the player revalidate his evidence
            root->evidence=root->evidence+number_of_witnesses;
            return root; 
        }
     
        //search right subtree
        if (root->pid < pid) 
            return search_insert_number(root->rc, pid,number_of_witnesses); 
  
        //search left subtree
        return search_insert_number(root->lc, pid, number_of_witnesses); 
    }

    //function that prints players and their evidence
    int print_players_evidence(struct Player* proot) {
        struct Player* pi=minPlayer(players_tree);
        int counter=1;
        while(pi!=players_sentinel){  //until players end print their evidence and their task
            printf("<Player %d, evidence %d> = ",counter,pi->evidence);
            struct Task* troot=pi->tasks;
            inorder_task(troot); 
            printf("\n");
            if(pi->pid==maxPlayer(players_tree)->pid)
                break;
            counter++;
            pi=nextOf(pi);    
        }
    }

//FOR EVENT S--------------------------------------------------------------------------------------------------------
    //function that returns the previous node of a node(predeccesor)
    struct Player* prevOf(struct Player* root){ 
        if (root == players_sentinel)
            return players_sentinel;

        //previous can be the rightest element of the left subtree of the node
        if (root->lc != players_sentinel) {
            root = root->lc;
            while (root->rc != players_sentinel)
                root = root->rc;
            return root;
        }

        struct Player* p = root->parrent;
        //or the parent root
        while (p != players_sentinel && p->lc == root) {
            root = p;
            p = p->parrent;
        }

        return p;
    } 

//FOR EVENT G
    //function to count all the nodes
    int counter_of_nodes(struct Player* pid1){
        struct Task* ti=pid1->tasks;
        int counter=0;
        while(ti!=NULL){ //coount=count+lcnt+1
            counter=counter+ti->lcnt+1;
            ti=ti->rc;
        }
        return counter;
    }
    //the split alogorithm
    struct Task* split(struct Player* pid1, struct Player* pid2){
        int cnt=0;
        struct Task* temp=minTask(pid1->tasks);
        struct Task* tree2=NULL;

        while(temp!=NULL){
            if(counter_of_nodes(pid1)%2==0){ //if  counter is even
                if(cnt<counter_of_nodes(pid1)/2){
                    pid2->tasks=insert_tree_task(pid2->tasks,temp->tid,temp->difficulty);
                    pid1->tasks=deleteANode(pid1->tasks,temp->tid);
                    cnt++;
                }
            }else{ //if is odd check ones more
                if(cnt<=counter_of_nodes(pid1)/2){
                    pid2->tasks=insert_tree_task(pid2->tasks,temp->tid,temp->difficulty);
                    pid1->tasks=deleteANode(pid1->tasks,temp->tid);
                    cnt++;
                }
            }
            temp=nextTask(pid1->tasks,temp);
        }
    }    

    //alternative delete node
    struct Task* dlt(struct Task* root, int tid){
        struct Task* tmp = NULL;

        //go all down to find the tid
        if(tid > root->tid)
            root->rc = dlt(root->rc,tid);
        else if(tid < root->tid)
            root->lc = dlt(root->lc, tid);
        //if it has 2 chilrens
        else if(root->lc != NULL && root->rc != NULL) {
            struct Task* minNode = minTask(root->rc);
            root->tid = minNode->tid;
            root->rc = dlt(root->rc,root->tid);
        }else {
            tmp = root;
            if(root->lc == NULL)
                root = root->rc;
            else if(root->rc == NULL)
                root=root->lc;
            free(tmp);
        }
        return root;
    }