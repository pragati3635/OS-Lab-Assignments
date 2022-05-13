Readme 




Q1)
There are 4 files for question 1 . 1st is input.c inorder to write  100 random nos. Upto 1000 in input.txt.
Download the folder .
Unzip it
On ubuntu terminal navigate to the folder.
Write the command “gcc input.c”
Once it compile.
Write the command “./a.out”
100 random numbers will be generated in input.txt file.
Now write the cmd “gcc mergesort.c”
Next write “./a.out input.txt output.txt”;
Case 1: If array is sorted it will print “it is already sorted”.
Case2 : If file is empty it will print “File is empty”.
Case3 : It will give the 100 numbers in sorted order in output.txt.


Q2) 
Download the folder .
Unzip it
On ubuntu terminal navigate to the folder.
write the command "gcc server.c -o snd" for compiling.
Then "./snd"
First enter an integer value for the number of clinets specifying how many clinets are playing the game.
Then enter the question(string to be send from server to the clients) for round 1.
Now for every client we need to open different terminal.
For each of the terminals opened for client write the command "gcc client.c -o rec".
Then "./rec"
First input for each clinet terminal should be an integer value which will give every clinet a client ID.
The question send by the server will be displayed on the terminal.
For every client enter the a string that is the answer to that question.
Scores after 1st round will be displayed on server terminal.