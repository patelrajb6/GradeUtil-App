/*gradeutil.cpp*/

//
// Raj Patel
// U. of Illinois, Chicago
// CS 341, Spring 2019
// Project #02: GradeUtil API
//

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "gradeutil.h"

using namespace std;

//
// API:
//

//
// ParseCourse:
//
// Parses a CSV (comma-separated values) line into a Course
// object, which is then returned.  The given line must have
// the following format:
//
//   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor
//
// Example:
//   BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington
//
// Note the lack of spaces, except perhaps in the title.
// If the given line does not have this format, the behavior
// of the function is undefined (it may crash, it may throw
// an exception, it may return).
//
Course ParseCourse(string csvline)
{
    string Dept, Number, Section, Title, A, B, C, D, F, I, NR, S, U, W, Instructor;
    stringstream ss(csvline);
    getline(ss, Dept, ',');
    getline(ss, Number, ',');
    getline(ss, Section, ',');
    getline(ss, Title, ',');
    getline(ss, A, ',');
    getline(ss, B, ',');
    getline(ss, C, ',');
    getline(ss, D, ',');
    getline(ss, F, ',');
    getline(ss, I, ',');
    getline(ss, NR, ',');
    getline(ss, S, ',');
    getline(ss, U, ',');
    getline(ss, W, ',');
    getline(ss, Instructor, ',');

    Course latest(Dept, Title, stoi(Number), stoi(Section), Instructor,
                  stoi(A), stoi(B), stoi(C), stoi(D), stoi(F), stoi(I), stoi(S), stoi(U), stoi(W), stoi(NR));

    return latest;
}

//
// GetDFWRate:
//
// Returns the DFW rate as a percentage for a given course,
// department, or college.  For a course whose grading type
// is defined as Course::Letter, the DFW rate is defined as
//
//   # of D grades + F grades + Withdrawals
//   -------------------------------------- * 100.0
//   # of A, B, C, D, F grades + Withdrawls
//
// The numerator is returned via the reference parameter DFW;
// the denominator is returned via the reference parameter N.
// If the course grading type is not Course::Letter, the DFW
// rate is 0.0, and parameters DFW and N are set to 0.
//
// When computed for a dept or college, all courses of type
// Course::Letter are considered in computing an overall DFW
// rate for the dept or college.  The reference parameters
// DFW and N are also computed across the dept or college.
//
double GetDFWRate(const Course &c, int &DFW, int &N)
{
    DFW = 0;
    N = 0;
    DFW = c.NumD + c.NumF + c.NumW;
    N = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW;
    if (N == 0 || DFW == 0 || c.getGradingType() != Course::Letter)
    {
        N = 0;
        DFW = 0;
        return 0.0;
    }
    double calculated = ((double)DFW / (double)N) * 100.0; // wow never knew this
    return calculated;
}

double GetDFWRate(const Dept &dept, int &DFW, int &N)
{
    DFW = 0;
    N = 0;
    double totalA = 0.0;
    double totalB = 0.0;
    double totalC = 0.0;
    double totalD = 0.0;
    double totalF = 0.0;
    double totalW = 0.0;
    double calculate = 0.0;
    for (auto &c : dept.Courses)
    {
        if (c.getGradingType() == Course::Letter)
        {
            totalA += c.NumA;
            totalB += c.NumB;
            totalC += c.NumC;
            totalD += c.NumD;
            totalF += c.NumF;
            totalW += c.NumW;
        }
    }
    DFW = totalD + totalF + totalW;
    N = totalA + totalB + totalC + totalD + totalF + totalW;
    if (N == 0 || DFW == 0)
    {
        N = 0;
        DFW = 0;
        return 0.0;
    }

    calculate = ((double)DFW / (double)N) * 100.0; // wow never knew this
    return calculate;
}

double GetDFWRate(const College &college, int &DFW, int &N)
{
    DFW = 0;
    N = 0;

    double totalA = 0.0;
    double totalB = 0.0;
    double totalC = 0.0;
    double totalD = 0.0;
    double totalF = 0.0;
    double totalW = 0.0;
    double calculate = 0.0;
    for (auto &d : college.Depts)
    {
        for (auto &c : d.Courses)
        {
            if (c.getGradingType() == Course::Letter)
            {
                totalA += c.NumA;
                totalB += c.NumB;
                totalC += c.NumC;
                totalD += c.NumD;
                totalF += c.NumF;
                totalW += c.NumW;
            }
        }
    }

    DFW = totalD + totalF + totalW;
    N = totalA + totalB + totalC + totalD + totalF + totalW;
    if (N == 0 || DFW == 0)
    {
        N = 0;
        DFW = 0;
        return 0.0;
    }

    calculate = ((double)DFW / (double)N) * 100.0; // wow never knew this
    return calculate;
}

//
// GetGradeDistribution
//
// Returns an object containing the grade distribution for a given
// course, dept or college.  For a course whose grading type is
// defined as Course::Letter, the grade distribution is defined by
// the following values:
//
//   N: the # of A, B, C, D, F grades
//   NumA, NumB, NumC, NumD, NumF: # of A, B, C, D, F grades
//   PercentA, PercentB, PercentC, PercentD, PercentF: % of A, B,
//     C, D, F grades.  Example: PercentA = NumA / N * 100.0
//
// If the course grading type is not Course::Letter, all values
// are 0.  When computed for a dept or college, all courses of
// type Course::Letter are considered in computing an overall
// grade distribution for the dept or college.
//
GradeStats GetGradeDistribution(const Course &c)
{
    GradeStats empty;
    if (c.getGradingType() == Course::Letter)
    {
        int n = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
        if (n == 0)
        {
            return empty;
        }
        double percentA = ((double)c.NumA / (double)n) * 100.0;
        double percentB = ((double)c.NumB / (double)n) * 100.0;
        double percentC = ((double)c.NumC / (double)n) * 100.0;
        double percentD = ((double)c.NumD / (double)n) * 100.0;
        double percentF = ((double)c.NumF / (double)n) * 100.0;
        GradeStats report(n, c.NumA, c.NumB, c.NumC, c.NumD, c.NumF, percentA, percentB, percentC, percentD, percentF);
        return report;
    }

    return empty;
}

GradeStats GetGradeDistribution(const Dept &dept)
{
    double totalA = 0.0;
    double totalB = 0.0;
    double totalC = 0.0;
    double totalD = 0.0;
    double totalF = 0.0;
    // double totalW=0.0;
    for (auto &c : dept.Courses)
    {

        if (c.getGradingType() == Course::Letter)
        {
            totalA += c.NumA;
            totalB += c.NumB;
            totalC += c.NumC;
            totalD += c.NumD;
            totalF += c.NumF;
        }

        //totalW+=c.NumW;
    }
    GradeStats empty;
    int n = totalA + totalB + totalC + totalD + totalF;
    if (n == 0)
    {
        return empty;
    }
    double percentA = ((double)totalA / (double)n) * 100.0;
    double percentB = ((double)totalB / (double)n) * 100.0;
    double percentC = ((double)totalC / (double)n) * 100.0;
    double percentD = ((double)totalD / (double)n) * 100.0;
    double percentF = ((double)totalF / (double)n) * 100.0;
    GradeStats report(n, totalA, totalB, totalC, totalD, totalF, percentA, percentB, percentC, percentD, percentF);

    return report;
}

GradeStats GetGradeDistribution(const College &college)
{

    double totalA = 0.0;
    double totalB = 0.0;
    double totalC = 0.0;
    double totalD = 0.0;
    double totalF = 0.0;
    // double totalW=0.0;
    for (auto &d : college.Depts)
    {
        for (auto &c : d.Courses)
        {

            totalA += c.NumA;
            totalB += c.NumB;
            totalC += c.NumC;
            totalD += c.NumD;
            totalF += c.NumF;
            //totalW+=c.NumW;
        }
    }

    GradeStats empty;
    int n = totalA + totalB + totalC + totalD + totalF;
    if (n == 0)
    {
        return empty;
    }
    double percentA = ((double)totalA / (double)n) * 100.0;
    double percentB = ((double)totalB / (double)n) * 100.0;
    double percentC = ((double)totalC / (double)n) * 100.0;
    double percentD = ((double)totalD / (double)n) * 100.0;
    double percentF = ((double)totalF / (double)n) * 100.0;
    GradeStats report(n, totalA, totalB, totalC, totalD, totalF, percentA, percentB, percentC, percentD, percentF);

    return report;
}

//
// FindCourses(dept, courseNumber)
//
// Searches the courses in the department for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, and returned in
// ascending order by section number.
//
vector<Course> FindCourses(const Dept &dept, int courseNumber)
{
    vector<Course> courses;

    for (auto &c : dept.Courses)
    {
        if (c.Number == courseNumber)
        {
            courses.push_back(c);
        }
    }
    sort(courses.begin(), courses.end(), [](Course i, Course j) {
        if (i.Section < j.Section)
        {
            return true;
        }
        else
        {
            return false;
        }
    });
    return courses;
}

//
// FindCourses(dept, instructorPrefix)
//
// Searches the courses in the department for those whose
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".
//
// If none are found, then the returned vector is empty.  If
// one or more courses are found, copies of the course objects
// are returned in a vector, with the courses appearing in
// ascending order by course number.  If two courses have the
// same course number, they are given in ascending order by
// section number.  Note that courses are NOT sorted by instructor
// name.
//
vector<Course> FindCourses(const Dept &dept, string instructorPrefix)
{
    vector<Course> courses;

    for (auto &c : dept.Courses)
    {
        if (c.Instructor.find(instructorPrefix) == 0)
        {
            courses.push_back(c);
        }
    }
    std::sort(courses.begin(), courses.end(), [](Course i, Course j) {
        if (i.Number < j.Number)
        {
            return true;
        }
        else if (i.Number > j.Number)
        {
            return false;
        }

        else
        {
            if (i.Section < j.Section)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    });

    return courses;
}

//
// FindCourses(college, courseNumber)
//
// Searches for all courses in the college for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department, then course number,
// and then section number.
//
vector<Course> FindCourses(const College &college, int courseNumber)
{
    vector<Course> courses;

    for (auto &d : college.Depts)
    {
        for (auto &c : d.Courses)
        {
            if (c.Number == courseNumber)
            {
                courses.push_back(c);
            }
        }
    }
    std::sort(courses.begin(), courses.end(), [](Course i, Course j) {
        if (i.Dept < j.Dept)
        {
            return true;
        }
        else if (i.Dept > j.Dept)
        {
            return false;
        }

        else if (i.Number < j.Number)
        {
            return true;
        }
        else if (i.Number > j.Number)
        {
            return false;
        }
        else
        {
            if (i.Section < j.Section)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    });
    return courses;
}

//
// FindCourses(college, instructorPrefix)
//
// Searches all the courses in the college for those whose
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by instructor, then course number,
// and then section number.
//
vector<Course> FindCourses(const College &college, string instructorPrefix)
{
    vector<Course> courses;
    for (auto &d : college.Depts)
    {
        for (auto &c : d.Courses)
        {
            if (c.Instructor.find(instructorPrefix) == 0)
            {
                courses.push_back(c);
            }
        }
    }

    std::sort(courses.begin(), courses.end(), [](Course i, Course j) {
        if (i.Instructor < j.Instructor)
        {
            return true;
        }
        else if (i.Instructor > j.Instructor)
        {
            return false;
        }

        else if (i.Number < j.Number)
        {
            return true;
        }
        else if (i.Number > j.Number)
        {
            return false;
        }
        else
        {
            if (i.Section < j.Section)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    });
    return courses;
}

vector<Course> FindCourses(const College &college, Course::GradingType type)
{
    vector<Course> courses;

    for (auto &d : college.Depts)
    {
        for (auto &c : d.Courses)
        {
            if (c.getGradingType() == type)
            {
                courses.push_back(c);
            }
        }
    }
    std::sort(courses.begin(), courses.end(), [](Course i, Course j) {
        if (i.Dept < j.Dept)
        {
            return true;
        }
        else if (i.Dept > j.Dept)
        {
            return false;
        }
        else
        {
            if (i.Number < j.Number)
            {
                return true;
            }
            else if (i.Number > j.Number)
            {
                return false;
            }
            else if (i.Section < j.Section)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    });
    return courses;
}

vector<Course> FindCourses(const Dept &dept, Course::GradingType type)
{
    vector<Course> courses;

    for (auto &c : dept.Courses)
    {
        if (c.getGradingType() == type)
        {
            courses.push_back(c);
        }
    }

    std::sort(courses.begin(), courses.end(), [](Course i, Course j) {
        if (i.Number < j.Number)
        {
            return true;
        }
        else if (i.Number > j.Number)
        {
            return false;
        }
        else
        {
            if (i.Section < j.Section)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    });
    return courses;
}
vector<Course> FindCoursesLetterA(const Dept &dept, double thres)
{
    vector<Course> courses;

    for (auto &c : dept.Courses)
    {
        if (c.letterA() > thres)
        {
            // cout<<c.letterA()<<c.Instructor<<endl;
            courses.push_back(c);
        }
    }

    std::sort(courses.begin(), courses.end(), [](Course i, Course j) {
        if (i.letterA() > j.letterA())
        {
            return true;
        }
        else if (i.letterA() < j.letterA())
        {
            return false;
        }
        else
        {
            if (i.Dept < j.Dept)
            {
                return true;
            }

            else if (i.Dept > j.Dept)
            {
                return false;
            }

            else if (i.Number < j.Number)
            {
                return true;
            }
            else if (i.Number > j.Number)
            {
                return false;
            }
            else
            {
                if (i.Section < j.Section)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    });
    return courses;
}

vector<Course> FindCoursesLetterA(const College &col, double thres)
{
    vector<Course> courses1;
    int d = 0, p = 0;
    for (auto &d : col.Depts)
    {
        for (auto &c : d.Courses)
        {
            if (c.letterA() > thres)
            {
                courses1.push_back(c);
            }
        }
    }

    std::sort(courses1.begin(), courses1.end(), [](Course i, Course j) {
        if (i.letterA() > j.letterA())
        {
            return true;
        }
        else if (i.letterA() < j.letterA())
        {
            return false;
        }
        else
        {
            if (i.Dept < j.Dept)
            {
                return true;
            }

            else if (i.Dept > j.Dept)
            {
                return false;
            }

            else
            {
                if (i.Number < j.Number)
                {
                    return true;
                }
                else if (i.Number > j.Number)
                {
                    return false;
                }
                else
                {
                    if (i.Section < j.Section)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    });
    return courses1;
}
vector<Course> FindCoursesthres(const Dept &dept, double thres)
{
    vector<Course> courses;

    for (auto &c : dept.Courses)
    {
        if (c.CourseDFWRate() > thres)
        {
            courses.push_back(c);
        }
    }

    std::sort(courses.begin(), courses.end(), [](const Course &i, const Course &j) {
        if (i.CourseDFWRate() > j.CourseDFWRate())
        {
            return true;
        }
        else if (i.CourseDFWRate() < j.CourseDFWRate())
        {
            return false;
        }
        else
        {
            if (i.Dept < j.Dept)
            {
                return true;
            }

            else if (i.Dept > j.Dept)
            {
                return false;
            }

            else if (i.Number < j.Number)
            {
                return true;
            }
            else if (i.Number > j.Number)
            {
                return false;
            }
            else
            {
                if (i.Section < j.Section)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    });
    return courses;
}

vector<Course> FindCoursesthres(const College &col, double thres)
{
    vector<Course> courses;
    int d = 0, p = 0;
    for (auto &d : col.Depts)
    {
        for (auto &c : d.Courses)
        {
            if (c.CourseDFWRate() > thres)
            {
                courses.push_back(c);
            }
        }
    }

    std::sort(courses.begin(), courses.end(), [](const Course &i, const Course &j) {
        if (i.CourseDFWRate() > j.CourseDFWRate())
        {
            return true;
        }
        else if (i.CourseDFWRate() < j.CourseDFWRate())
        {
            return false;
        }
        else
        {
            if (i.Dept < j.Dept)
            {
                return true;
            }

            else if (i.Dept > j.Dept)
            {
                return false;
            }

            else if (i.Number < j.Number)
            {
                return true;
            }
            else if (i.Number > j.Number)
            {
                return false;
            }
            else
            {
                if (i.Section < j.Section)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    });
    return courses;
}
