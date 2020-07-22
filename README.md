<h1>grep commannd implementation</h1>

Options Description:<br>
-r : Read all files  under each directory,recursively.<br>
-c : This prints only a count of the lines that match a pattern.<br>
-h : Display the matched lines, but do not display the filenames.<br>
-H : Display filename as well.<br>
-i : Ignores, case for matching.<br>
-v : This prints out all the lines that do not matches the pattern.<br>
-f file : Takes patterns from file, one per line.<br>
 -q : Do not write anything to standard output.Exit immediately with zero status if any match is found, even  if  an error  was  detected.<br>
-b : Print the 0-based byte offset within the input file before  each line of output.<br>
-m : Stop reading a file after n matching lines. Takes one extra argument of n.<br>
-w : Search pattern as a complete word.<br>
-e exp : Can use multiple times to take multiple patterns.Useful for patterns starting with '-'.To grep regular expressions( '^', '$', '.', '?').
(any combination of cases can be done)<br>
