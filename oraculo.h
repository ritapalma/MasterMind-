/* Interface of Oracle for MasterMind project */
/* 2018/19 - 2Semester                        */
/* Author: PROG team                          */

/* Type definition for output of function validate_key */
/* The client must declare a pointer to a variable of  */
/* this type, like so:                                 */
/* reply * answer;                                     */
/* answer = validate_key(...);                         */
typedef struct _reply reply;

/* Initializes variables for oracle     */
/* Needs: size of key, number of colors,*/
/* and if repeat is allowed             */
/* s must be 4 to 8                     */
/* c must be 6 to 12                    */
/* r must be either 0 (no repeats) or 1 */
void activate_oracle(int s, int c, int r);

/* Generates and saves a random key according to specs      */
/* Overwrites new key if one had been previously produced   */
/* It forces exit if the client calls this function without */
/* activating the oracle                                    */
/* If argument is diferent from zero, generated key will be */
/* displayed                                                */
void generate_key(int);

/* Produces the result for an attempt to find the saved key */
/* try is produced by the client                            */
/* If the client produces an attempt with wrong colors,     */
/* returns NULL.                                            */
/* Example: if number of colors is 6, key size is 4, and    */
/* this function receives an attempt with AAAG, it will     */
/* return NULL because G is the seventh color.              */
reply * validate_key(char * try);

/* Produces the result for two keys, comparing them         */
/* try1 and try2 are produced by the client                 */
/* Both try1 and try2 should be of same size as specified   */
/* in the activation. This function will simply check for   */
/* the given size. Anything above or below will get NULL    */
/* Example: if number of colors is 6, key size is 4, and    */
/* this function receives one try with AAAG, it will        */
/* return NULL because G is the seventh color.              */
reply * compare_keys(char * try1, char * try2);

/* Shuts down the oracle, by freeing all allocated variables */
void terminate_oracle();

/* Returns the number of white pegs */
/* rep is produced by function      */
/* validate_key                     */
/* Returns -1 when rep is NULL      */
int get_whites(reply * rep);

/* Returns the number of black pegs */
/* Returns -1 when rep is NULL      */
int get_blacks(reply * rep);
