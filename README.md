# Lightweight_Userinput_Processing

8/14/2026
This is a small project of mine to include a simple program collection for artificial intelligence text reading. Basically, this allows a user to input something then matches their input to an answer contained in a database. This provides a simple efficient method to replace other language processing tools. 

The text files are setup to be expanded past their original source and the database file, large_sources in this repository, is used as the container of information. 

BETA3.1.2.py: Acts as the frontend in this file where it will dissect the user input to be properly read and passed to the backend C file. The backend file is what creates tree associations to be able to check every pathway in the database. The text files can easily be expanded upon with limited modifications or data processing in order for this program to work properly.

***READ BEFORE MODIFYING***
The backend file likely will need directly changes in it to work correctly. The C file executes on a machine running Linux Mint. 
The backend file also creates the JSON files in the default directory. This was left as I understand your configuration will likely have some specified need for these file locations.
