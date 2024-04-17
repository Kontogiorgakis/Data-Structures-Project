/**************************************************
 *                                                *
 * file: among_us.h                               *
 *                                                *
 *                                                *
 * @Author  Skerdi Basha                          *
 * @Version 1-12-2020                             *
 * @email   sbash@csd.uoc.gr                      *
 *                                                *
 * @brief   Header file for the needs of CS-240   *
 * project 2020, with the structures and function *
 * prototypes                                     *
 *                                                *
 **************************************************
 */

#ifndef __AMONG_US_H_
#define __AMONG_US_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/**
 * Structure defining a node of the players double linked tree
 * tree
 */
struct Player
{
    int pid;                        /*Player's identifier*/
    int is_alien;                   /*Alien flag*/
    int evidence;                   /*Amount of evidence*/
    struct Player *parrent;         /*Pointer to the parent node*/
    struct Player *lc;              /*Pointer to the left child node*/
    struct Player *rc;              /*Pointer to the right child node*/
    struct Task *tasks;             /*Pointer to the head of player's task tree*/
};

/**
 * Structure defining a node of the tasks sorted tree
 */
struct Task
{
    int tid;                        /*Task's identifier*/
    int difficulty;                 /*Task's difficulty*/
    int lcnt;                       /*Node's left child's node counter*/
    struct Task *lc;                /*Pointer to the left child node*/
    struct Task *rc;                /*Pointer to the right child node*/ 
};

struct HT_Task
{
    int tid;                        /*Task's identifier*/
    int difficulty;                 /*Task's difficulty*/
    struct HT_Task *next;           /*Pointer to the next node*/
};

struct General_Tasks_HT
{
    int count;                      /*Count of tasks*/
    struct HT_Task **tasks;         /*General tasks hash table*/
};

struct Completed_Tasks_PQ
{
    int size;                       /*Count of completed tasks*/
    struct HT_Task *tasks;         /*Completed tasks priority queue*/
};

unsigned int max_tasks_g; /* Max number of tasks */
unsigned int max_tid_g;   /* Max task tid */

/* Primes for your universal hashing implementation */
extern unsigned int primes_g[650];

/* Global variable, pointer to the head of the players tree */
struct Player *players_tree;

/* Global variable, pointer to the head of the tasks tree */
struct General_Tasks_HT general_tasks_ht;

/* Global variable, pointer to the top of the completed task's priority queue */
struct Completed_Tasks_PQ *completed_tasks_pq;

/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize();

/**
 * @brief Register Player
 *
 * @param pid The player's id
 *
 * @param is_alien The variable that decides if he is an alien or not
 * @return 1 on success
 *         0 on failure
 */
int register_player(int pid, int is_alien);

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
int insert_task(int tid, int difficulty);

/**
 * @brief Distribute Tasks to the players
 * @return 1 on success
 *         0 on failure
 */
int distribute_tasks();

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
int implement_task(int pid, int tid);

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
int eject_player(int pid_1, int pid_2);

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
int witness_eject(int pid_1, int pid_2, int pid_a, int number_of_witnesses);

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
int sabotage(int number_of_tasks, int pid);

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
int vote(int pid_1, int pid_2, int vote_evidence);

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
int give_work(int pid_1, int pid_2);

/**
 * @brief Terminate
 *
 * @return 1 on success
 *         0 on failure
 */
int terminate();

/**
 * @brief Print Players
 *
 * @return 1 on success
 *         0 on failure
 */
int print_players();

/**
 * @brief Print Tasks
 *
 * @return 1 on success
 *         0 on failure
 */
int print_tasks();

/**
 * @brief Print Priority Queue
 *
 * @return 1 on success
 *         0 on failure
 */
int print_pq();

/**
 * @brief Print Players & Task tree
 *
 * @return 1 on success
 *         0 on failure
 */
int print_double_tree();

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void);

//MY FUNCTIONS
struct Player* insert_player(struct Player* node, struct Player* new);
void inorder(struct Player* root);

//FOR EVENT T
int hash(int tid);

//FOR EVENT D
struct Player* minPlayer(struct Player* node);
struct Player* maxPlayer(struct Player* node);
struct Player* nextOf(struct Player* n);
struct Task* insert_tree_task(struct Task* node, int tid ,int dif);
struct Task* newTask(int tid, int dif);
void lcounter(struct Task* root);
void inorder_task(struct Task* root);
void inorder_player_task(struct Player* proot) ;

//FOR EVENT I
struct Player* search_player(struct Player* root, int pid);
struct Task* deleteANode(struct Task *root, int pid);
struct Task* search_task(struct Task* root, int key);
    //FOR PRIORITY QUEUE FUNCTIONS
    void insert_task_pq(struct HT_Task task) ;
    void swap(struct HT_Task *p,struct HT_Task *q);
    void sort(struct HT_Task *arr);
    void delete();

//FOR EVENT E
struct Player* delete_player(struct Player* root, int pid);    
struct Player* insert_pid2(struct Task* carrier, struct Player* root,int pid);
struct Task* change_tasks(struct Task* curr, struct Task* this);
    //manipulators
    struct Task* nextTask(struct Task* root, struct Task* n);
    struct Task* maxTask(struct Task* node);
    struct Task* minTask(struct Task* node);

//FOR EVENT W
struct Player* search_insert_number(struct Player* root, int pid, int number_of_witnesses) ;
int print_players_evidence(struct Player* proot);
//just testing
void print2DUtil(struct Player *root, int space) ;
//FOR EVENT S
struct Player* prevOf(struct Player* root);
//FOR EVENT G
int counter_of_nodes(struct Player* pid1);
struct Task* split(struct Player* pid1, struct Player* pid2);
struct Task* dlt(struct Task* root, int tid);

#endif /* __AMONG_US_H_ */
