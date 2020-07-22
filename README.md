Title : Implement grep commannd
Name : Hrishikesh Sambhaji Chavan
MIS : 111703012

Options Description:
-r : Read all files  under each directory,recursively.
-c : This prints only a count of the lines that match a pattern.
-h : Display the matched lines, but do not display the filenames.
-H : Display filename as well.
-i : Ignores, case for matching.
-v : This prints out all the lines that do not matches the pattern.
-f file : Takes patterns from file, one per line.
 -q : Do not write anything to standard output.Exit immediately with zero status if any match is found, even  if  an error  was  detected.
-b : Print the 0-based byte offset within the input file before  each line of output.
-m : Stop reading a file after n matching lines. Takes one extra argument of n.
-w : Search pattern as a complete word.
-e exp : Can use multiple times to take multiple patterns.Useful for patterns starting with '-'.To grep regular expressions( '^', '$', '.', '?').
(any combination of cases can be done)