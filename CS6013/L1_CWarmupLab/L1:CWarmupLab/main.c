/*********************************************************************
 *
 * Corinne Jones
 * CS6013
 * L1: C Warmup Lab due 1/15/24
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> // For strlen()
#include <stdbool.h> //for bools
#include <assert.h> // for assert statements

/*********************************************************************
 *
 * The C functions in this lab use patterns and functionality often found in
 * operating system code. Your job is to implement them.  Additionally, write some test
 * cases for each function (stick them in functions [called xyzTests(), etc or similar])
 * Call your abcTests(), etc functions in main().
 *
 * Write your own tests for each function you are testing, then share/combine
 * tests with a classmate.  Try to come up with tests that will break people's code!
 * Easy tests don't catch many bugs! [Note this is one specific case where you may
 * share code directly with another student.  The test function(s) from other students
 * must be clearly marked (add '_initials' to the function name) and commented with the
 * other student's name.
 *
 * Note: you may NOT use any global variables in your solution.
 *
 * Errata:
 *   - You can use global vars in your testing functions (if necessary).
 *   - Don't worry about testing the free_list(), or the draw_me() functions.
 *
 * You must compile in C mode (not C++).  If you compile from the commandline
 * run clang, not clang++. Submit your solution files on Canvas.
 *
 *********************************************************************/

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 *
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 * Ah, the joys of using bitwise operators!
 *
 * Hint: you may want to write helper functions for these two functions
 *
 *********************************************************************/

//helper function which sorts in descending order
//pass through a pointer to a and b to pass by value, then dereference it to subtract the values
int compare (unsigned char *a, unsigned char *b) {
    return (*b - *a);
}

unsigned long byte_sort( unsigned long arg )
{
    unsigned char array[8]; //array to store the hex nums, unsigned char d/t size (i.e., 8 bits/1 byte), can be many types but would need to keep size in mind
    int arraysize = 8;
    unsigned long result = 0;  //unsigned long to be returned
    
    //goes through the array of chars, right shifts i*8 (i.e., the number of bits to get it in the right most position), '&' with hexadecimal FF to keep the right most bits but clear the rest out, stores that number into the array at [i]
    for (int i=0; i<8; i++){
        array[i]=(arg>>(i*8))&0xFF;
    }
    
    //qsort is a built in sort function, provide the array to be sorted, the size of the array, the byte size of the array items, and a compare function
    qsort(array, arraysize, sizeof(unsigned char), compare);
    
    for (int i=0; i<8; i++)
    {
        result <<= 8; //this shifts the previous result to make room for the new one
        result|=array[i]; //or the result with the array at [i] to keep the items in the array since the result will be 0 at that point
    }
    
    //return the sorted unsigned long
    return result;
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 *
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

unsigned long nibble_sort (unsigned long arg)
{
    unsigned char arr[16]; //initialize the variable
    int arraysize = 16;
    
    for (int i=0; i<16; i++){
        arr[i]=(arg>>(i*4))&0xF;//right shift i*4 to just shift a nibble size, mask nibble
    }
    
    qsort(arr, arraysize, sizeof(unsigned char), compare);
    
    unsigned long result = 0;
    
    for (int i=0; i<16; i++)
    {
        result <<= 4;  //shifts the previous result to make room for the new one
        result|=arr[i];//or result (0's) with arr[i] to set correct bits
    }
    
    return result;
}

/*********************************************************************/

typedef struct elt {
  char val;
  struct elt *link;
} Elt;

/*********************************************************************/

/* Forward declaration of "free_list()"... This allows you to call   */
/* free_list() in name_list() [if you'd like].                       */
void free_list( Elt* head ); // [No code goes here!]

/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 * - you must use the "name" variable (change it to be your name).
 *
 * Note, since we're using C, not C++ you can't use new/delete!
 * The analog to delete is the free() function
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *
 * Implement print_list and free_list which should do what you expect.
 * Printing or freeing a nullptr should do nothing.
 *
 * Note: free_list() might be useful for error handling for name_list()...
 *
 *********************************************************************/

Elt* name_list(){
    char * name = "Corinne";
    Elt* head=NULL;
    Elt* previous=NULL;
    
    for(int i=0; i<strlen(name); i++)
    {
        Elt* current=malloc(sizeof(Elt)); //allocate memory for Elt
        
        if(i==0){
            head=current; //first loop through will set the head as current
        }
        
        if(current==NULL){
            free_list(head);//if ever not enough memory, free the list
            printf("out of memory");
            return NULL;
        }

        current->val=name[i]; //set the current value to the correct string index
        
        if(previous!=NULL){
            previous->link=current; //will run for all but the first loop
        }
        
        previous=current; //keep track of the previous, update the previous to the one just initialized
    }
    
   return head; //return the first node
}

/*********************************************************************/

void print_list( Elt* head )
{
    //loop through until the end printing the value, then update the head
    while(head!=NULL){
        printf("%c", head->val);
        head=head->link;
    }
}
/*********************************************************************/

void free_list( Elt* head )
{
    //loop through the whole list, deleting each head
    while(head!=NULL){
        Elt* next=head->link;
        free(head);
        head=next;
    }
}

/*********************************************************************
 *
 * draw_me()
 *
 * This function creates a file called 'me.txt' which contains an ASCII-art
 * picture of you (it does not need to be very big).
 *
 * Use the C stdlib functions: fopen, fclose, fprintf, etc which live in stdio.h
 * - Don't use C++ iostreams
 *
 *********************************************************************/

void draw_me()
{
    //initialize the file
    FILE *myPic;
    
    //dictates the path to store the result
    myPic = fopen("/Users/corinnejones/GitHubSchool/Spring2024/CS6013/L1_CWarmupLab/MyPicture.txt", "w");
    //print to the file
    fprintf(myPic, "  ^_^\n");
    fprintf(myPic, "(>'.'<)\n");
    fprintf(myPic, "(U   U)\n");
    fprintf(myPic, "(^)__(^)\n");
    
    //close the file
    fclose(myPic);
    
}

/*********************************************************************
 *
 * Test Code - Place your test functions in this section:
 */

 bool testByteSort() {
     unsigned long hex = 0x0403deadbeef0201;
     unsigned long actual=0xefdebead04030201;
     unsigned long result=0;
     
     result=byte_sort(hex);
     
     printf("\nThe result for byte_sort() is: ");
     printf("%lx\n", result);
     
     return actual==result;
 }

bool testByteSort2() {
    unsigned long minValue = 0;
    unsigned long minSorted = byte_sort(minValue);
    if (minSorted == 0){
        return true;
    }
    return false;
}

bool testNibbleSort() {
    unsigned long hex = 0x0403deadbeef0201;
    unsigned long actual=0xfeeeddba43210000;
    unsigned long result=0;
    
    result=nibble_sort(hex);
    
    printf("\nThe result for nibble_sort() is: ");
    printf("%lx\n", result);
    
    return actual==result;
}

bool testNibbleSort2() {
    unsigned long minValue = 0;
    unsigned long minSorted = nibble_sort(minValue);
    if (minSorted == 0){
        return true;
    }
    return false;
}

bool testNameListCreation(){
    Elt* list;
    Elt* current;
    char actual[] = "Corinne";
    char result[8];
    
    list=name_list();
    current=list;
    
    while(current!=NULL){
        char temp[] = {current->val, '\0'}; //All strings end in a string terminator
        strcat(result, temp); // Concatenate the character to the result
        current = current->link; //update current
    }
    
    return strcmp(actual, result) == 0; //compare the strings, true if the same
}

bool testNameListDeletion(){
    Elt* list;
    int size;
    
    list=name_list();
    free_list(list);
    
    if(list->val=='C'){
        return false;
    }
    
    return true;
}

bool testDrawMe(){
    char actual[] = ("  ^_^");
    char result[6];
    int size = sizeof(result);
    
    draw_me();
    
    FILE* myPic = fopen("/Users/corinnejones/GitHubSchool/Spring2024/CS6013/L1_CWarmupLab/MyPicture.txt", "r");
    if (myPic == NULL) {
        return false;
    }
    fgets(result, size, myPic);
    
    return strcmp(actual, result) == 0;
}

/*********************************************************************
 *
 * Lindsay Haslam's tests:
 */
 void testByteSort_LH() {
    //Normal scenario.
    unsigned long test_val1 = 0x0403deadbeef0201;
    unsigned long expected1 = 0xefdebead04030201;
    assert(byte_sort(test_val1) == expected1);

    //All bytes are the same.
    unsigned long test_val2 = 0x1111111111111111;
    unsigned long expected2 = 0x1111111111111111;
    assert(byte_sort(test_val2) == expected2);

    //Obvious increasing order.
    unsigned long test_val3 = 0x0102030405060708;
    unsigned long expected3 = 0x0807060504030201;
    assert(byte_sort(test_val3) == expected3);

    //Already in decreasing order.
    unsigned long test_val4 = 0x0807060504030201;
    unsigned long expected4 = 0x0807060504030201;
    assert(byte_sort(test_val4) == expected4);

    printf("BYTE SORT: All tests passed.\n");
}

void testNibbleSort_LH(){

        //Normal scenario
        unsigned long test_val1 = 0x0403deadbeef0201;
        unsigned long expected1 = 0xfeeeddba43210000;
        assert(nibble_sort(test_val1) == expected1);

        //All nibbles are the same.
        unsigned long test_val2 = 0x1111111111111111;
        unsigned long expected2 = 0x1111111111111111;
        assert(nibble_sort(test_val2) == expected2);

        //Increasing order of nibbles
        unsigned long test_val3 = 0x0123456789abcdef;
        unsigned long expected3 = 0xfedcba9876543210;
        assert(nibble_sort(test_val3) == expected3);

        //Already in decreasing order
        unsigned long test_val4 = 0xfedcba9876543210;
        unsigned long expected4 = 0xfedcba9876543210;
        assert(nibble_sort(test_val4) == expected4);

        printf("NIBBLE SORT: All tests passed.\n");
    }

/*********************************************************************
 *
 * Sarah Bateman's tests:
 */
//testing it will fill in extra 0's
bool testByteSort2_SB() {
    unsigned long result = byte_sort(0x05);
    return result == 0x500000000000000;
}
//testing example given in instructions
bool testNibbleSort1_SB() {
    unsigned long result = nibble_sort(0x0403deadbeef0201);
    return result == 0xfeeeddba43210000;
}
//testing if I give it an input that's already correct it won't change the order
bool testNibbleSort2_SB() {
    unsigned long result = nibble_sort(0xFEDCBA9876543210);
    return result == 0xfedcba9876543210;
}

//testing letters in my name
bool testNameList_SB() {
    Elt *nameList = name_list();
    //checking first letter
    if(nameList->val != 'C'){
      return false;
    }
    //moving pointer
    nameList = nameList->link;
    //checking third letter
    if(nameList->val != 'o'){
      return false;
    }
    //moving pointer
    nameList = nameList->link;
    //checking last letter
    if(nameList->val != 'r'){
      return false;
    }
    //moving pointer
    nameList = nameList->link;
    //checking last letter
    if(nameList->val != 'i'){
      return false;
    }
    //moving pointer
    nameList = nameList->link;
    //checking last letter
    if(nameList->val != 'n'){
      return false;
    }
    //moving pointer
    nameList = nameList->link;
    //checking last letter
    if(nameList->val != 'n'){
      return false;
    }
    //moving pointer
    nameList = nameList->link;
    //checking last letter
    if(nameList->val != 'e'){
      return false;
    }
    //moving pointer
    nameList = nameList->link;
    //checking after 'e' the result is null
    if(nameList != NULL){
      return false;
    }
    //if none of them failed then return true
    return true;
}

//testing free_list works
bool testFreeList_SB(){
  //create a linkedlist with my name
  Elt *nameList = name_list();
  //call free list to clear it
  free_list(nameList);
  //check that the head doesn't have the original value anymore
   if(nameList->val == 'C'){
      return false;
    }
    else{
      return true;
    }
}


/*********************************************************************
 *
 * main()
 *
 * The main driver program.  You can place your main() method in this
 * file to make compilation easier, or have it in a separate file.
 *
 *
 *********************************************************************/

int main()
{
    assert(testByteSort());
    assert(testByteSort2());
    assert(testNibbleSort());
    assert(testNibbleSort2()); 
    assert(testNameListCreation());
    assert(testNameListDeletion());
    assert(testDrawMe());
    
    testByteSort_LH();
    testNibbleSort_LH();
    
    
     if (testByteSort2_SB()) {
           printf("byte_sort passed - Test 2\n");
     } else {
         printf("byte_sort failed - Test 2.\n");
     }
     if (testNibbleSort1_SB()) {
           printf("nibble_sort passed - Test 1\n");
     } else {
         printf("nibble_sort failed - Test 1.\n");
     }
     if (testNibbleSort2_SB()) {
           printf("nibble_sort passed - Test 2\n");
     } else {
         printf("nibble_sort failed - Test 2.\n");
     }
     if (testNameList_SB()) {
           printf("name_list passed \n");
     } else {
         printf("name_list failed \n");
     }
     if (testFreeList_SB()) {
           printf("free_list passed \n");
     } else {
         printf("free_list failed \n");
     }

    
}
