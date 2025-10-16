## Project 3 - part 1
## Student Information
Name: Jaime Yah-Perez 
Student ID: 008856387
git: https://github.com/JaimeYahPerez/project3_part1

## Collaboration and Resources
Research for ASCII tables and their relations with int and char variables was done through web searches into
stack overflow and the ascii-code website. which helped me realize that int and char integer values were fairly flexible
when it came to comparisons for the ASCII helper functions

Wben it came to the initial setup of the scanner constructor I was initially confused at what to expect 
with the given file inputs from main.cpp, which led to the usage of chatgpt on the instructions in main in order to fully
understand the logic, i/o, and interaction between main and scanner.cpp for the purpose of the project implementations

Ontop of that, I would also use Chatgpt for primarily troubleshooting and understanding proper usage of file related
libraries like filesystem, iostream, and fstream, since I still do not feel like I understood these libraries well when
writing initially setting up the Scanner constructor and first tokenize() function

code suggestions were taken from chatgpt during debugging, and this is present in readWord() with the EOF comparisons
for end of file checks, and the use of peek() to check the next letter for validating apostrophes in conjuctions

When I originally wrote both tokenize functions I did as much as I could manually, until I overheard what functions we
had access to in utils, therefore what is present in both is primarily a result of what resources were present
in the given starter files, and how I best interpreted the project specs

## Implementation Details
Several helper functions were made for the sake of identifying only the necessary ASCII letters for tokenization
as stated by the requirements
is_ascii() converts a given integer into the form a char through a static_cast, and compares it to the number 127
which is the end of the ascii table, to see if the given character is an ascii value

is_ascii_letter() checks a given integer ch, and verifies if it is an ascii code value with a call to is_ascii()
if it returns true, it carries on with a static_cast conversion to char variable u, and compares it with letters 
'A', 'Z','a', and 'z', to see if the given char lies within these letter ranges on the ascii table
it returns true

to_ascii_letter() checks if the given int ch lies within the capital letters range A-Z on the ascii table
and returns the ascii integer ch in the equivalent lowercase ascii range. If it does not lie in that capital range
it will simply return the input as the output

is_apostrophe() checks if the given integer ch is equivalent to the ascii apostrophe integer

Scanner() reduces the given filesystem input path into its string form through string(), followed up with just the file 
name without the extension through stem(). and records this into string base. 
It then tacks on the ".txt" extension onto base, and appends this to the inputPath's parent path through parent_path()
and records this into inputpath_

tokenize(words) first clears the given vector of any elements as a safety measure due to the presence of &
Due to the assumption that we have to check for potential errors that are possible in utils in our scanner, I made
error_type st to serve as a record of errors for a directory and file error, which are checked sequentially
upon opening through iftsream in, another check is made once more to ensure that the inputPath has successfully opened

within the while loop, a call to readWord is made with input in to being recording words present in the input .txt file
this is done with move as transfer of string variables from string w, to our words vector for later tokenization
the loop will then break upon seeing string w is empty, and afterward a return with NO_ERROR is done

overloaded tokenize() calls the first tokenize function with input vector & words, if any error is returned, that error is
passed in an if() check immediately after. If not, writeVectorToFile() is called with the now filled words vector and 
the given output file, with the call being put in return in case of error returns. If no error occurs, then NO_ERROR is 
returned, and the output .tokens file is written

readWord() starts with a string declaration of tok, which holds recorded characters. Followed with a call to in.get() for 
the current character, which is recorded into int variable c.
If it is found to be the end of file(EOF), it is immediately returned as is

if the character is not ASCII, a check is made to see if tok is empty, if yes it is returned, if not, in.get() is called
for the next character

if the character is an apostrophe, a check is made to see if tok is empty, if yes the next character is called with 
in.get(). if not, a new variable is made called nxt, with a call to in.peek().
If the next character is not the end of file, is an ascii character, and is an ascii letter
then the apostrophe is pushed into tok, in.get() is called, and the char in nxt is made lowercase while being pushed into
tok. And the next character is called for into c and the loop is sent into the next iteration

if the character is simply an ascii letter, then it is pushed back into tok as a lowercase letter, and the next
character is called for into c, and the loop is continued

As an additional safety check if no previous checks have passed, but tok is not empty, then it is returned as is
Otherwise, the next character is called for into c
 

## Testing & status
Testing on blue has proved successful with the provided scripts, apart from the provided TheBells.txt which shows no differences
with diff, but does display an error afterwards when looking for the output .tokens file, if ran using the given bash
files. 




