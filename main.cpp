#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "GradeUtil.h"
#include "GradeUtil.cpp"
using namespace std;

string numtoGradetype(Course::GradingType type)
{
    string ar[3] = {"letter", "satisfactory", "unknown"};
    return ar[type];
}
void VecDisplay(const vector<Course> &dis)
{
    for (auto &e : dis)
    {
        int DFW = 0, N = 0;

        cout << e.Dept << " " << e.Number << " (section " << e.Section << "): " << e.Instructor << endl;
        cout << " # students: " << e.getNumStudents() << endl;
        cout << " course type: " << numtoGradetype(e.getGradingType()) << endl;
        GradeStats Stats = GetGradeDistribution(e);
        auto DFWSummary = GetDFWRate(e, DFW, N);
        printf(" grade distribution (A-F): %0.2f%%, %0.2f%%, %0.2f%%, %0.2f%%, %0.2f%% \n", Stats.PercentA, Stats.PercentB, Stats.PercentC, Stats.PercentD, Stats.PercentF);
        cout << " DFW rate: " << DFWSummary << "%" << endl;
    }
}
void totalSummary(const College &col)
{
    int DFW = 0, N = 0;
    static int numCourses = col.getNumClasses();
    GradeStats Stats = GetGradeDistribution(col);
    auto DFWSummary = GetDFWRate(col, DFW, N);
    auto totalStudents = col.getNumStudents();
    cout << "# of courses taught: " << numCourses << endl;
    cout << "# of students taught: " << totalStudents << endl;
    printf("grade distribution (A-F): %0.2f%%, %0.2f%%, %0.2f%%, %0.2f%%, %0.2f%% \n", Stats.PercentA, Stats.PercentB, Stats.PercentC, Stats.PercentD, Stats.PercentF);
    cout << "DFW rate: " << DFWSummary << "%" << endl;
}

void totalSummary(const Dept &dept)
{
    int DFW = 0, N = 0;
    int numCourses = dept.getNumClasses();
    GradeStats Stats = GetGradeDistribution(dept);
    auto DFWSummary = GetDFWRate(dept, DFW, N);
    auto totalStudents = dept.getNumStudents();
    cout << dept.Name << ":" << endl;
    cout << " #  courses taught: " << numCourses << endl;
    cout << " #  students taught: " << totalStudents << endl;
    printf(" grade distribution (A-F): %0.2f%%, %0.2f%%, %0.2f%%, %0.2f%%, %0.2f%% \n", Stats.PercentA, Stats.PercentB, Stats.PercentC, Stats.PercentD, Stats.PercentF);
    cout << " DFW rate: " << DFWSummary << "%" << endl;
}

void summaryFunct(const College &col)
{
    string sum;
    cout << "dept name, or all? ";
    cin >> sum;
    if (sum == "all")
    {
        for (auto &e : col.Depts)
        {

            totalSummary(e);
        }
        // cout<<endl;
        return;
    }
    auto iter = find_if(col.Depts.begin(), col.Depts.end(), [=](const Dept &a) {
        if (a.Name == sum)
        {
            return true;
        }
        else
        {
            return false;
        }
    });

    if (iter == col.Depts.end())
    {
        cout << "**dept not found" << endl;
        return;
    }
    //cout<<iter->Name<<":"<<endl;
    totalSummary(*iter);
}

void searchFunc(const College &col)
{
    string Coursenum, search;
    vector<Course> searched;
    cout << "dept name, or all? ";
    cin >> search;
    cout << "course # or instructor prefix? ";
    cin >> Coursenum;
    if (search == "all" && isdigit(Coursenum[0]) == true)
    {
        searched = FindCourses(col, stoi(Coursenum));
        if (searched.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(searched);
    }
    else if (search == "all" && isdigit(Coursenum[0]) == false)
    {
        searched = FindCourses(col, Coursenum);
        if (searched.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(searched);
    }
    else if (isdigit(Coursenum[0]) == true)
    {
        auto iter = find_if(col.Depts.begin(), col.Depts.end(), [=](const Dept &a) {
            if (a.Name == search)
            {
                return true;
            }
            else
            {
                return false;
            }
        });
        if (iter == col.Depts.end())
        {
            cout << "**dept not found" << endl;
            return;
        }
        searched = FindCourses((*iter), stoi(Coursenum));
        if (searched.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(searched);
    }
    else
    {
        auto iter = find_if(col.Depts.begin(), col.Depts.end(), [=](const Dept &a) {
            if (a.Name == search)
            {
                return true;
            }
            else
            {
                return false;
            }
        });
        if (iter == col.Depts.end())
        {
            cout << "**dept not found" << endl;
            return;
        }
        searched = FindCourses((*iter), Coursenum);
        if (searched.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(searched);
    }
}

void unknownFunc(const College &col)
{
    string unknown;
    vector<Course> foundUnknown;
    cout << "dept name, or all? ";
    cin >> unknown;
    if (unknown == "all")
    {
        foundUnknown = FindCourses(col, Course::Unknown);
        if (foundUnknown.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(foundUnknown);
    }
    else
    {
        auto iter = find_if(col.Depts.begin(), col.Depts.end(), [=](const Dept &a) {
            if (a.Name == unknown)
            {
                return true;
            }
            else
            {
                return false;
            }
        });
        if (iter == col.Depts.end())
        {
            cout << "**dept not found" << endl;
            return;
        }
        foundUnknown = FindCourses((*iter), Course::Unknown);
        if (foundUnknown.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(foundUnknown);
    }
}
void dfwFunc(const College &col)
{
    vector<Course> dfw;
    string Inputdfw;
    double threshold = 0.0;
    cout << "dept name, or all? ";
    cin >> Inputdfw;
    cout << "dfw threshold? ";
    cin >> threshold;
    if (Inputdfw == "all")
    {
        dfw = FindCoursesthres(col, threshold);
        if (dfw.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(dfw);
    }
    else
    {
        auto iter = find_if(col.Depts.begin(), col.Depts.end(), [=](const Dept &a) {
            if (a.Name == Inputdfw)
            {
                return true;
            }
            else
            {
                return false;
            }
        });
        if (iter == col.Depts.end())
        {
            cout << "**dept not found" << endl;
            return;
        }
        dfw = FindCoursesthres((*iter), threshold);
        if (dfw.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(dfw);
    }
}
void letterAFunc(const College &col)
{
    vector<Course> lettera;
    lettera.clear();
    string InputA;
    double thresholdA = 0.0;
    cout << "dept name, or all? ";
    cin >> InputA;
    cout << "letter A threshold? ";
    cin >> thresholdA;
    if (InputA == "all")
    {
        lettera = FindCoursesLetterA(col, thresholdA);

        if (lettera.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(lettera);
    }
    else
    {
        auto iter = std::find_if(col.Depts.begin(), col.Depts.end(), [=](const Dept &a) {
            if (a.Name == InputA)
            {
                return true;
            }
            else
            {
                return false;
            }
        });
        if (iter == col.Depts.end())
        {
            cout << "**dept not found" << endl;
            return;
        }
        lettera = FindCoursesLetterA((*iter), thresholdA);
        if (lettera.size() == 0)
        {
            cout << "**none found" << endl;
            return;
        }
        VecDisplay(lettera);
    }
}

void commands(string a, const College &col)
{
    if (a == "summary")
    {
        summaryFunct(col);
    }
    else if (a == "search")
    {
        searchFunc(col);
    }
    else if (a == "unknown")
    {
        unknownFunc(col);
    }
    else if (a == "dfw")
    {
        dfwFunc(col);
    }
    else if (a == "letterA")
    {
        letterAFunc(col);
    }
    else if (a == "#")
    {
        return;
    }
    else
    {
        //cout << endl;
        cout << "**unknown command" << endl;
    }
}

int main()
{
    cout << fixed;
    cout << setprecision(2);

    string fileName, line, UserCommand, j;

    cin >> fileName;

    ifstream file(fileName);
    if (!file.good())
    {
        cout << "cannot open file" << endl;
        return -1;
    }

    string col, year, semester;
    getline(file, line);
    stringstream ss(line);
    getline(ss, col, ',');
    getline(ss, semester, ',');
    getline(ss, year, ',');
    cout << "** College of " << col << ", " << semester << " " << year << " **" << endl;
    getline(file, line);
    // cout<<line<<endl;

    College Eng(col);

    while ((getline(file, line)))
    {
        Course p = ParseCourse(line);

        j = p.Dept; //got the course

        auto mp = find_if(Eng.Depts.begin(), Eng.Depts.end(), [=](const Dept &m) {
            if (m.Name == j)
            {
                return true;
            }
            else
            {
                return false;
            }
        });                        //make function       searched if the dept existed or not
        if (mp == Eng.Depts.end()) //if it doesnt exist than create new dept into the college
        {
            Dept bc(j);

            bc.Courses.push_back(p);

            Eng.Depts.push_back(bc);
        }
        else //else if the dept does exist than insert the course directly into the Dept
        {
            mp->Courses.push_back(p);
        }
    }

    std::sort(Eng.Depts.begin(), Eng.Depts.end(), [](const Dept &d, const Dept &e) {
        return (d.Name < e.Name);
    });

    totalSummary(Eng); //Total college details
    cout << endl;
    cout << "Enter a command> ";
    cin >> UserCommand;

    commands(UserCommand, Eng);
    while (UserCommand != "#")
    {
        cout << "Enter a command> ";
        cin >> UserCommand;

        commands(UserCommand, Eng);
    }
}