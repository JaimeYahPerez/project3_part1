[## Project 3 - final
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

# Part 2
The header files utilize the provided sketches from the main project page, and
the tree node header file is pulled from a previous lab with edits made to it for usability in this project

For this part, I utilized cppreference to get a better understanding of the usage of std::optional and std::string_view
as these were some new declarations to me, and would be used later in the definitions of class functions

Significantly more debugging was done this time around, especially when it came to main.cpp and its' interaction with
the testing scripts. This led to the use of chatgpt to determine what the error that was being output meant and where 
any issues may lie. Which would lead to the current implementation of main.cpp

main() has now been readjusted to correctly send the input file to scanner. And the given implementation used for 
project 1 was reworked to fit with the test scripts given.

*Bin Search Tree*
The implementations done for this heavily based off what was described in the main project page

int cmp() was written to help with needed comparison tests based off of previous experience working with trees, and it
takes an input of string_view just to further see how interactions work with this

The constructor and public functions were written using prior experience and lab works

insertHelper(), inorderhelper(), sizeHelper(), heightHelper, and findNode() were designed based off our work with
recursion and have logic similar to those labs

*Priority Queue*
This implementation was also based heavily on what we have done before, specifically more so with our recent lab and 
lectures. The project page also helped guide the implementation of the sorting needed

The constructor and insert() were mostly based on the description given in the main project page

The public functions were based on previous experience working with the TreeNode lab

isSorted() is also based on work done with vectors previously

higherPriority() is based on what was given in the main project page

# Final Part
The Huffman Tree implementation is primarily based off of the given sketch in the main project page. And some parts of 
the function implementations is based off of what was done previously.
Further info was gotten off of our previous lab involving huffman trees, and our lecture covering this part of the
project.

Certain deviations are made from the project outline this time around. Though these are minor with the intention of 
maintaining better functionality

This can be seen with the encode function's implementation, and now further parts of main().

Debugging was done with use of compiler errors from blue, code analysis from Clion, and chatgpt analysis. 

*Huffman Tree*
BuildFromCounts() works off of the outline we were given from the project page, and the info we were given in lecture.

The destructor and destroy() were based off of part 2 and the BinSearchTree destructor

assignCodes() is based off of previous work in part 2 

writeHeader() is based off of previous work from part 2 and part 1. Combining both now to account for errors

encode() works off of the outline we were given in the main project page, but does deviate partially from it. Such as 
the use of a vector<pair> instead of a map, and the call to assignCodes() instead of VisitLeavesWithCodes 
(which is not in the provided sketch). This was due to a desire to simplify the initial collection of codes as 
assignCodes() taking an input of vector<&pair>, therefore I felt a similar data structure would be better
The rest of it is based on what we were given, and some syntax was used from research into the ostream library, such as
with .put() and .good()

assignCodesDFS() utilizes logic from our previous work with recursion

writeHeaderPreorder() functions similarly to assignCodesDFS()

*main*
main() had further restructuring done to it, this time to accomodate the needed paths for the output files. This 
suggestion came from chatgpt to help keep the flow of main readable and smooth. The huffman tree related additions were 
added at the end 

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
 
# part 2
The needed implementations for BinSearchTree and priorityqueue were provided by main project page, and some of the 
structures for their implementation was based off what we were given.

TreeNodes was pulled from a previous lab, and edited to accommodate the needs for this project part

*BinSearchTree*
cmp() was made as a helper function for later comparisons, which compares the strings a and b directly, and returns a 
number -1,0, 1 based on which is bigger. -1 string a is less than b. 0 both are equivalent, 1 a is bigger than b

The destructor makes a call to the private destroy() function for cleanup
insert() makes a call to the private inserthelper()
bulkInsert() makes a continuous call to insertHelper() with the given vector input

destroy() returns when node is null. if not null, it makes a recursive call to the left subtree() of node, then a 
recursive call to the right subtree() of node, and once both have been made, will destroy the current node, and return

contains() makes a call to findNode() and returns whether or not it returns a nullptr
countOf() makes a call to findNode() and stores whatever it returns regarding the given word. If null, it returns 
nullopt, if not it instead returns the int value of the returned node

inorderCollect() clears the given &out vector, and then makes a call to inorderHelper using that directly passed &out
size() and height() make direct calls to their respective helper functions which will pass on their returns

insertHelper() creates a new node when the current node is null, and sets it value to 1 and returns.
This was the base case, if not at the base case, a call to the cmp() function is made with the current node->key() and
the current word and stores the outcome in int test.
This then leads to a switch using test, and a case is chosen based on the outcome. if 0 the word is already present and 
the frequency is increased by 1, if -1 then we make a recursive call to go left, or right if 1. This is to keep 
traversing until we find the word again, or reach a null node, allowing the tree to remain sorted during insertion

findNode() uses a similar recursion logic, with a base case of null node returning nullptr, meaning the node was not
found. otherwise, a call to cmp() is made comparing word and the current node's key. The outcome is sent into a switch
which returns the node if the word is found, or a left or right recursive call is made for traversal

inorderHelper() uses recursive logic to traverse the tree using inorder search, and emplaces the node key and value into
the output vector once leftsubtree traversal is no longer possible

sizeHelper() uses recursive logic to travel left then right, and adds 1 to the return statement to count present nodes
heightHelper() uses recursive logic to traverse left then right, and compares the count made by both recursive traversal
to see which branch is larger, and returns that branches count

 *priority queue*
higherPriority() works off the logic given in the main project page
isSorted() runs a for loop which checks if the higherpriority is maintained throughout the vector
and returns false if it isnt, and true if the for loop is completed with no issue

the constructor moves all elements in the input nodes into items_. Then a check is made to make sure more than 1 element 
is present. If so, then a while loop is ran for as long as i is less than the elements present in items_, and during 
this, an insertion sort algorithm is ran using the current element of items_ to maintain sorted status

size() and empty() returns their respective function calls

findmin() first checks if items is empty, if so it returns null, otherwise it returns the last element of items_
extractMin() does a similar check first, then it records the last element for return before popping it

deleteMin() checks if empty first, then pops the last element

insert checks if the given node is null first for safety. Then it pushes back the node into items_, before setting up 
for an insertion sort that starts at the end of the vector, and iterates backwards to sift up the new element
to maintain sorted status

print() does a standard print to console

*main*

the start of main has been edited to ensure the correct parent path to input_output/ is made to ensure the .txt file
is found

a new bst is declared later to parse through the provided words vector to create the new binary search tree. results are
then recorded into freqLex utilzing inordercollect, and data is extracted utilizing a for loop and std::max() for 
console output. the newly made bst is then used to output data made from the input file into the consoler

freqLex is then used to pass data into a newly made TreeNode leaves, which is then used for a new declaration of 
priorityqueues pq. Afterwards a new frequency list output file is made, while checking to make sure this output file is
accessible. Lastly, a buffer vector is made called buf of vector<string, int> to extract data from pq for later writing
a for loop is then ran which writes data in buf into the output frequency file, while also adding 10 spaces first to 
satisfy the test scripts

# part 3
*Huffman Tree*
buildFromCounts() Due to the importance of the class actually owning the tree, it is made to initialize a vector of nodes
and to run a for loop converting elements from the counts vector into leaf tree nodes for our tree. also stored in a 
vector of nodes. This vector is then used to create a priorityqueue, which is checked for 2 edge cases. if there are no 
elements in the priority queue, then the current huffman tree has its' root pointer set to null and returns. If one 
element is present then it and it alone is extracted from the priority queue and is set as the root pointer of the tree
and returned. For queue's with more than 2 elements, they are sent into a while loop where 2 mins are extracted, and are
used to construct the parent based on the combined values of the 2 mins. After, the key of the parent is chosen based on 
whichever is the smaller word. the two mins are then attached to the left and right subtree and the node is then inserted
into the priority queue. After completion of the new priority queue, the min is set as the root of the huffman tree and 
is returned.

The destructor makes a call to the destroy helper function which then handles deletion of the tree. Afterwards, it 
cleans up by setting the root to null

assignCodes() clears the output vector for safety, declears a prefix string variable, and makes a call to assignCodesDFS

writeHeader() does a preflight check to make sure the passed ostream is usable for writing. It then makes a call to 
writeHeaderPreorder, with an additional check to the ostream current state to see if the file failed to write, and then
finally returning no_error if all was successful

encode() first checks to see if the passed ostream is usable. afterwhich a vector of string pairs is declared and will 
receive codes from the assignCodes function call. then a range for loop is made spanning the tokens vector, and a string
pointer called bits is declared as null at the start. A second range for loop is declared to span the range of available 
codes. if the current token is found, then its' code is recorded into bits and the second loop is broken. If no token is 
found then a short circuit is triggered and an error returned. Another range for loop is then entered spanning bits, and
every char of bits if inserted into our provided os_bits. If nothing is entered a short circuit error is triggered. Then 
for char in bits, a col counter is incremented and if it reaches 80, a new line is insrted into os_bits and the counter 
is set back to 0. os_bits during this time outputs to the console as it is an ostream. Should the main for loop end with 
col not being 0, a new line is inserted with another check for another succesful ostream occcurring. If no issues occur 
then no_error is returned

destroy() utilizes recursion and a post order search to delete nodes

assignCodesDFS() utilizes depth first search to construct the prefix with 0 and 1 being pushed back depending on the 
branch traversal. If both branches lead to a nullptr, then a code is assigned based on the current prefix, or 0 if the 
tree node leads to a single word tree.

writeHeaderPreorder() functions similarly to assignCodesDFS, only that the if check outputs whatever the prefix has 
recorded

## Testing & status
Testing on blue has proved successful with the provided scripts, apart from the provided TheBells.txt which shows no differences
with diff, but does display an error afterwards when looking for the output .tokens file, if ran using the given bash
files. 

# part 2
Significant testing was done to ensure that the current implementation works well with the given bash scripts.
Currently the last test ran on blue gave back 22 matches with the current implementation, and no errors

# part 3
Testing on blue has proven successful and all tests resulted in matches for a total of 44.
