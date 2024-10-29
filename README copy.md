## Instructions for Compilation and Execution

1. Compile and Run the Code:
   To compile all the files and execute the program, use the following command:
   ```
   make execute
   ```

2. Clean Project Directory:
   To remove the object files and clean up the project directory, use the command:
   ```
   make clean
   ```

3. Customize Time Slice for Round Robin:
   The default time slice for Round Robin is set to 1. To customize the time slice for Round Robin, use the following command:
   ```
   make execute RR_TIME_SLICE="1"
   ```
   Replace "1" with your desired time slice value.

4. To save the output in the file, and to execute the binary file use the following command:
    ```
   make execute > output.txt
   ```
  