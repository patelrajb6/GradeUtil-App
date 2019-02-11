# GradeUtil-App
Building a console app which shows the grade data of given college

This program takes in .csv file which are comma separated. The file contains information about 
the Courses taught and there grade data.
The csv file is in order  of Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor.
This csv file is then parsed into string stream and Course object is created.
Course object is than inserted into the Dept object and then Dept object is inserted into the
College object.

There are five command of the program.
summary-this command gives you the summary of either whole college
or you can type in particular Dept to get the particular summary of that dept.

