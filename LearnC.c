#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>   
#include <string.h>
#include <time.h>

#define LENGTH_WORD_MAX 21
#define LENGTH_SENTENCE_MAX 207
#define MAX_WORDS_IN_SENTENCE_GENERATION 33

typedef struct{
    char word [LENGTH_WORD_MAX];
    int occurances;
    int chance;
}WrodProbability; 


typedef struct {
    char word [LENGTH_WORD_MAX];
    int occurances;
    int num_of_next_words;
    WrodProbability * next_words;
}WordStruct;


WordStruct * arr_of_WordStruct;
int arr_of_WordStruct_len = 0;


void free_all_memory()
{
    //Free the attribute: next_words array,  of every WordStruct object
    for (int i = 0; i < arr_of_WordStruct_len; i++)
    {
        free(arr_of_WordStruct[i].next_words);
    }

    //Free the arr_of_WordStruct which "holds" all the WordStruct objects
    free(arr_of_WordStruct);
}


int get_random_number(int len)
{
    int k = rand()%len;
    return k;
}


//Checking the CommandLine arguments and saving them to the coresponding derefrenced pointer.
void check_argc(int argc, char **argv, int *pNum_of_tweets, char file_path[], int *pWords_to_read)
{
    if (argc < 3)
    {
        puts("Usage {num of tweets} {file path} {num of word to read}");
        exit(EXIT_FAILURE);
    }

    *pNum_of_tweets = atoi(argv[1]);
    strcpy_s(file_path, 100, argv[2]);

    if (argc == 4)
    {
        *pWords_to_read = atoi(argv[3]);
    }
    else if (argc == 3)
    {
        *pWords_to_read = 2147483647;
    }
    else
    {
        puts("Usage {num of tweets} {file path} {num of word to read}");
        exit(EXIT_FAILURE);
    }
}


//Creating a WordStruct object for each "first occurance" of a word.
//Adding the new object to the arr_of_WordStruct.
void add_word_to_arr_of_WordStruct(char token[])
{
    //Add 1 to arr_of_WordStruct_len,  increase the size of arr_of_WordStruct
    arr_of_WordStruct_len += 1;
    arr_of_WordStruct = (WordStruct *)realloc(arr_of_WordStruct, arr_of_WordStruct_len * sizeof(WordStruct));
    if (arr_of_WordStruct == NULL)
    {
        puts("Error: can't allocate memory in heap");
        exit(EXIT_FAILURE);
    }

    //Create a new WordStruct object.
    //Set its attributes 
    WordStruct ws;
    ws.occurances = 1;
    ws.num_of_next_words = 0;
    strcpy_s(ws.word, LENGTH_WORD_MAX, token);
    ws.word[strlen(ws.word)] = '\0';
    ws.next_words = malloc(ws.num_of_next_words * sizeof(*ws.next_words));
    if (ws.next_words == NULL)
    {
        puts("Error: can't allocate memory in heap");
        exit(EXIT_FAILURE);
    }

    //Add the object to arr_of_WordStruct
    arr_of_WordStruct[arr_of_WordStruct_len - 1] = ws;
    //###What words were added: printf("\nAdded:%s", arr_of_WordStruct[arr_of_WordStruct_len - 1].word);
}



//Adding a word to the: next_words dynamic array,  of the corresponding WordStruct object
int add_to_next_words(WordStruct * ws, char token[])
{
    //Iterate through all the ws.next_words array and see if this word is all ready there.
    //If its there:  add 1 to its occurances,    Calculate chance again.
    for (int i = 0; i < ws->num_of_next_words; i++)
    {
        if (strcmp(token, ws->next_words[i].word) == 0)
        {
            ws->next_words[i].occurances += 1;
            ws->next_words[i].chance = ws->next_words[i].occurances/ws->occurances;
            return 1;
        }
    }
    
    //Else: create a new instance of WordProbability,  assign values to its attributes,   add it to the last index of ws.next_words
    WrodProbability wp;
    strcpy_s(wp.word, LENGTH_WORD_MAX, token);
    wp.word[strlen(wp.word)] = '\0';
    wp.occurances = 1;
    wp.chance = wp.occurances/ws->occurances;

    ws->num_of_next_words += 1;
    ws->next_words = realloc(ws->next_words, (ws->num_of_next_words) * sizeof(*ws->next_words));
    ws->next_words[ws->num_of_next_words - 1] = wp;
    return 2;
}



//1) Gets a word
//2) iterates through arr_of_WordStruct.word and checks if that word is already there.
int iterate_through_words_of_sentence(char sentence[], int * pNum_of_words_we_read, int words_to_read)
{
    char delimiters[] = " \n";
    char * token;
    int i = 0, equal = 1, index_of_word_we_operate_on;
    char last_char_of_word;

    token = strtok(sentence, delimiters);

    while (token != NULL)
    {
        //Check if read enough words all ready.
        *pNum_of_words_we_read += 1;
        if (words_to_read - 1 < *pNum_of_words_we_read)
        {
            return 1;
        }
    
        equal = 1;

        //iterates through arr_of_WordStruct.word and checks if that word is already there.
        //If it is there: add 1 to its occurances,   set the flag to 0,   save the index,   break out of loop.
        for (i = 0; i < arr_of_WordStruct_len; i++)
        {
            if (strcmp(token, (arr_of_WordStruct+i)->word) == 0)
            {
                (arr_of_WordStruct+i)->occurances += 1;
                equal = 0;
                index_of_word_we_operate_on = i;
                break;
            }
        }

        //If the flag is 1, it means that the word isn't in the arr_of_WordStruct yet. 
        // We will save the index of the last arr_of_WordStruct element,   add the word to arr_of_WordStruct.
        if (equal)
        {
            add_word_to_arr_of_WordStruct(token);
            index_of_word_we_operate_on = arr_of_WordStruct_len - 1;
        }

        //Checks if the word ends with '.'.
        //If it is: Set arr_of_WordStruct[index_of_word_we_operate_on].next_words = NULL,   get the next string.
        if(token[strlen(token) - 1] == '.')
        {
            arr_of_WordStruct[index_of_word_we_operate_on].next_words = NULL;
            token = strtok(NULL, delimiters);
        }
        //if it isn't: Get the next word,   add it to the arr_of_WordStruct[index_of_word_we_operate_on].next_words
        else
        {
            token = strtok(NULL, delimiters);
            if (token != NULL)
            {
                add_to_next_words(&arr_of_WordStruct[index_of_word_we_operate_on], token);
            }
        }
    }
    return 0;
}



//Get the last char of a string, to check if its '.'
char get_last_char_of_str(char str[])
{
    char last;
    last = str[strlen(str)-1];
    return last;
}



/* Unlike the "Hebrew University Project" which gave  all the words in the next_word array   same chance to be chosen, regarless to the number of occurances.
Here I wanted to Create an array which contains the index numbers of the next_words array * their occurances.
Example for how the array would look like:   1,1,1,2,3,3,4,4,4,4,4,5,6......
Now we will randomly choose an index from this array (for example [5] which equals to 3) and return the 3.
This techniqe allows us to take into consideration the number of occurances of a word.
*/
const char * get_random_word_from_next_words_dynamic_array(WordStruct ws)
{
    int counter = 0;
    int * arr_of_indexes = (int *)malloc(counter * sizeof(int));
    if (arr_of_indexes == NULL)
    {
        puts("Error: can't allocate memory in heap");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < ws.num_of_next_words; i++)
    {
        for (int j = 0; j < ws.next_words[i].occurances; j++)
        {
            counter += 1;
            arr_of_indexes = realloc(arr_of_indexes, counter * sizeof(int));
            arr_of_indexes[counter - 1] = i;
        }
    }
    int index = arr_of_indexes[get_random_number(counter)];
    free(arr_of_indexes);
    return ws.next_words[index].word;
}


//Get a word that was taken from next_words array of the previous WordStruct and find its correponding WordStruct object.
//Return the index number of the correponding WordStruct object in the arr_of_WordStruct.
int find_WordStruct_object_of_corresponding_word(char word[])
{
    for (int i = 0; i < arr_of_WordStruct_len; i++)
    {
        if (strcmp(word, arr_of_WordStruct[i].word) == 0)
        {
            return i;
        }
    }
}


int main(int argc, char **argv)
{
    int num_of_tweets, words_to_read;
    char file_path[100];
    srand(time(NULL));
    
    check_argc(argc, argv, &num_of_tweets, file_path, &words_to_read);
    //####Checking the arguments##### printf("\n%d\n%s\n%d", num_of_tweets, file_path, words_to_read);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        puts("Error: Unable to open file");
        exit(EXIT_FAILURE);
    }

    char sentence[LENGTH_SENTENCE_MAX];
    arr_of_WordStruct = (WordStruct *)malloc(arr_of_WordStruct_len * sizeof(WordStruct));
    if (arr_of_WordStruct == NULL)
    {
        puts("Error: can't allocate memory in heap");
        exit(EXIT_FAILURE);
    }

    int num_of_words_we_read = 0, flag = 0;
    int * pNum_of_words_we_read = &num_of_words_we_read;
    while (1)
    {
        fgets(sentence, LENGTH_SENTENCE_MAX, fp);
        flag = iterate_through_words_of_sentence(sentence, pNum_of_words_we_read, words_to_read);

        if(feof(fp) || flag == 1)
        {
            break;
        }
    }
    fclose(fp);

    //Choosing the first word
    char first_word[LENGTH_WORD_MAX];
    int random_number = get_random_number(arr_of_WordStruct_len);

    int tweeted_words_counter = 0;
    //Generating XYZ amount of tweets
    for (int i = 0; i < num_of_tweets; i++)
    {
        tweeted_words_counter = 0;
        //Checking the first word doesn't end with '.' 
        while(1)
        {
            strcpy_s(first_word, LENGTH_WORD_MAX, arr_of_WordStruct[random_number].word);
            if (get_last_char_of_str(first_word) != '.')
            {
                break;
            }

            //If the word ends with '.', get another word after if random_number pointed to the last index.
            if (random_number + 1 == arr_of_WordStruct_len)
            {
                random_number -= 1;
            }
            else
            {
                random_number += 1;
            }
        }
        printf("\n\n[+] %s ", first_word);
        tweeted_words_counter += 1;


        while (get_last_char_of_str(first_word) != '.' && tweeted_words_counter <= MAX_WORDS_IN_SENTENCE_GENERATION)
        {
            strcpy_s(first_word, LENGTH_WORD_MAX, get_random_word_from_next_words_dynamic_array(arr_of_WordStruct[random_number]));
            
            printf("%s ", first_word);
            tweeted_words_counter += 1;

            //This is not a random Number anymore, but the Index of first_word in arr_of_WordStruct[]
            random_number = find_WordStruct_object_of_corresponding_word(first_word);
        }
    
    }
    
    free_all_memory();

    return 1;

}
