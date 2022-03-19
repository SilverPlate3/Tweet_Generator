# Tweet_Generator
Tweet generator in C.\
I found the requirements of a project given by “Hebrew university in Jerusalem” in C and decided to give it a try. \
The project relies on nested structures and nested dynamic arrays combined.\
After finishing that project with their guiding lines, I wanted to modify a few things and improve the final product.\
unlike their project, the program takes into consideration the number of occurrences of each word and number of occurrences  of their following words in the text when actually choosing the next words to tweet.


# Text requirements:
* Every line must end with a dot. There could be many sentences with a dot in 1 line. (see text examples)
* Characters must be “understood” by your compiler, I found out that my GCC compiler had issue with many characters including dashes. It will replace these characters with garbage values.
* You must know what’s the character length of the longest line and longest word in the text. Update it in the program, under the corresponding #define\
#define LENGTH_WORD_MAX XYZ\
#define LENGTH_SENTENCE_MAX XYZ\
I provided these details in the first lines of each text.


# Important to acknowledge:
* Must be compiled into an .exe (I used GCC).
* Command line arguments should be: {number of tweets} {path to .txt file}     **Example** : a.exe 3 brady.txt
* If your using the text’s I provided, don’t expect tweets with much logic… I just gave it random sentences that I copied from short stories & other .txt files. 
Nevertheless, this script should work on every .txt file that complies with the text requirements mentioned above.

