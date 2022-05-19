//1. Read all the student’s details from the given files and import them into your
//programme. (4 Marks).
//2. Develop a perfect shuffle algorithm(coded) that will give the output in the table
//above. (6 Marks).
//3. Print out the final lists into two separate CSV files having ICS A and B. (6 Marks).
//4. Include lucrative features into your programme such as options, presentable
//output and generally a well - structured system. (4 Marks).

//Importing necessary libraries
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>
#include <time.h>
#include <algorithm>
#include <random>

using namespace std;

//Defining custom types
typedef string String;
typedef vector<String> CSVRow;      //vector to store fields
typedef CSVRow::const_iterator CSVRowCI;
typedef vector<CSVRow> fileContents;//Vector to vector to store each row as a vector of fields
typedef fileContents::const_iterator CSVDatabaseCI;

//Function prototyping
void readCSV(string fname, fileContents& db, char delim);
void streamFile(string fileName, fstream& file);
void concact(fileContents& vec, fileContents& loc);
fileContents shuffler(fileContents& vector1, fileContents& vector2, fileContents& vector3);

void write_csv(fileContents& tobesplit, string fname);
void sortStudents(fileContents& tobesorted);
void userOutput();



//Class display Grid
//Default Call:displayGrid(fileContents& file, string fileName) 
//Functions:printGrid()
//          setgridsize(int x, int y)
//          preview(rows = 10; cols = 2,printGrid())
// 
//Variables: int x,y
//           string fname
//           fileContents printable
class displayGrid 
{
    int rows, cols;
    string fname;
    fileContents printable;
    public:

        //Default Call
    displayGrid(fileContents& file, string fileName) 
    {
        rows = file.size(); cols = 3;
        fname = fileName; 
        printable = file;
    };
    void printGrid();

    //Custom sized grid
    void setgridsize(int x, int y)
    {
        rows = x; cols = y; printGrid(); 
    };
    void preview() 
    {
        rows = 10; cols = 2;
        printGrid();
    }

};

/*Function Name:printGrid - namespace displayGrid
Parameters: none
Function type: void
Prints a grid filled with the fields with specified column headers
Prints row headers based on fileContents[][0] 
*/
void displayGrid::printGrid()
{
    cout << "\n";
    cout << "\t\t" << fname << "\t\t" << "\n";

    //String for titles of the output
    string colNames[4] = { "    Adm No""   ""Surname""\t""Other Names""\t  ""Gender" };
    for (int i = 0; i < cols; i++)
    {
        if (!i)
        {
            cout << colNames[i];
        }
        else
        {
            cout << "  " << colNames[i];
        }
    }
    cout << "  " << colNames[3] << "\n\n";


    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (!j)
            {
                cout <<i<<"). "<< printable[i][0] << " || ";
            }
            else
            {
                cout << printable[i][j] << " || ";
            }
        }
        cout << printable[i][3] << "\n";

    }
    cout << "\n";
}


/*main function
variables:char delimiter
         :string ICSA
         :string ICSB
         :string ICSC
         :fileContents icsGrpA
         :fileContents icsGrpB
         :fileContents icsGrpC
         :fileContents all_students
*/
int main() 
{
  //variables initialization
    char delimiter = ',';
    string ICSA = "inputICSA.csv";
    string ICSB = "inputICSB.csv";
    string ICSC = "inputICSC.csv";
    fstream icsA, icsB, icsC;
    fileContents icsGrpA, icsGrpB, icsGrpC;

    //Reading predefined files
    readCSV(ICSA, icsGrpA, delimiter);
    readCSV(ICSB, icsGrpB, delimiter);
    readCSV(ICSC, icsGrpC, delimiter);

    //Shuffles the specified files
    fileContents all_students = shuffler(icsGrpA, icsGrpB, icsGrpC);
    //Clearing memory taken up by the fileContents icsGrpA,icsGrpB,icsGrpC
    icsGrpA.clear();
    icsGrpA.shrink_to_fit();
    icsGrpB.clear();
    icsGrpB.shrink_to_fit();
    icsGrpC.clear();
    icsGrpC.shrink_to_fit();
    fileContents newGroupA, newGroupB;

    //Writing all_students to allstudents.csv file
    write_csv(all_students, "allstudents.csv");
    //Call to sortStudents function
    sortStudents(all_students);
    displayGrid allStudents(all_students, "all Students ");
    //Gives the user output options
    userOutput();
    return 0;
}

/*Function Name:streamFile
* Function type:void
* Function Paramters:string fileName
*                    fstream& file
* Function variable:None
* parses csv files into a fstream
*/
void streamFile(string fileName, fstream& file)
{
    file.open(fileName);
    //Try-catch block to get any errors while opening the file
    try
    {
        if (!file.is_open())
        {
            throw runtime_error("Could not open file");
            exit(0);
        }
    }
    catch (const exception& e)
    {
        cerr << e.what() << '\n';
    }


}
/*Function Name:readCSV
* Function type:void
* Function parameters:string fname
*                     fileContents& filecontent
*                     char delim
* Function variables:String csvLine
*                    fstream fileStream
* Accepts an fstream as a parameter and populates a fileContents variable with contents of the fstream
*/
void readCSV(string fname, fileContents& filecontent, char delim)
{
    //Variable initialization
    String csvLine;
    fstream fileStream;
    streamFile(fname, fileStream);

    // read every line from the stream
    //populating fileContent with fields and rows read from fstream
    while (getline(fileStream, csvLine))
    {
        istringstream csvStream(csvLine);
        CSVRow csvRow;
        String csvCol;

        // read every element from the line that is seperated by commas
        // and put it into the vector or strings
        while (getline(csvStream, csvCol, delim))
        {
            csvCol.erase(remove(csvCol.begin()
                , csvCol.end(), '\"'), csvCol.end());
            csvRow.push_back(csvCol);
        }
        filecontent.push_back(csvRow);
        //deleting csvRow
        csvRow.clear();
        csvRow.shrink_to_fit();
    }

    fileStream.clear();
    
}
/*Function Name:concact
*          type:void
*          parameters:fileContents& vec
*                     fileContents& loc
* Accepts two fileContents vectors and concacts(joins) them into one ,loc
*/
void concact(fileContents& vec, fileContents& loc)
{
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        loc.push_back(vec[i]);
    }

}

/*Function Name:Shuffler
*          Type:fileContents
*    Parameters:fileContents& vector1
*               fileContents& vector2
*               fileContents& vector3
*           
*     Variables:int sample_size
*               fileContents total_pool
* Shuffles three flowcharts variables and returns one with the contents of all of them shuffled in one
*/
fileContents shuffler(fileContents& vector1, fileContents& vector2, fileContents& vector3)
{
    int sample_size = vector1.size() + vector2.size() + vector3.size();
    fileContents total_pool;
    concact(vector1, total_pool);
    concact(vector2, total_pool);
    concact(vector3, total_pool);
    auto engine = default_random_engine{};
    shuffle(begin(total_pool), end(total_pool), engine);
    return total_pool;

}


/*Function Name: write_csv
*          Type: void
*    Parameters: fileContents& tobewritten
*                string fname
*     Variables: ofstream myFile
*                int vsize
* Writes contents of fileContents passed as a parameter and writes to a csv file with the name passed as a parameter as well
*/
void write_csv(fileContents& tobewritten, string fname)
{
    ofstream myFile(fname);
    int vsize = tobewritten.size();
    for (int i = 0; i < vsize; ++i)
    {
        for (int j = 0; j <= 3; ++j)
        {
            if (j != 3)
            {
                myFile << tobewritten[i][j] << ",";
            }
            else
            {
                myFile << tobewritten[i][j];
            }
        }
        myFile << "\n";
    }
    myFile.close();
}


/*function Name: sortStudents
*          Type: void
*     Parameter:fileContents& tobesorted
*     Variables:fileContents grpB
*               vector <int> locs
*               int size
*                   femsize
*                   tobesortedsize
*                auto engine
* Takes a single fileContents variable and sorts it into two groups;
* Genrated 4 csv files during the operation, two based on gender and two randomly shuffled groups
*/
void sortStudents(fileContents& tobesorted)
{
    fileContents grpB;
    vector <int> locs;
    int size = tobesorted.size();
    for (int i = 0; i < size; ++i)
    {
        if (tobesorted[i][3] == "F")
        {
            CSVRow female;
            for (int j = 0; j <= 3; j++)
            {
                female.push_back(tobesorted[i][j]);
            }
            grpB.push_back(female);
            locs.push_back(i);
            female.clear();
        }
    }
    write_csv(grpB, "femaleStudents.csv");
    for ( unsigned int i = 0; i < locs.size(); ++i)
    {
        fileContents::iterator itr = tobesorted.begin();
        tobesorted.erase(itr + (locs[i] - i));
    }
    write_csv(tobesorted, "Malestudents.csv");

    int femsize = grpB.size();
    int tobesortedsize = tobesorted.size();
    for (int j = femsize - 1; j >= femsize / 2; j--)
    {
        CSVRow temp;
        for (int k = 0; k <= 3; k++)
        {
            temp.push_back(grpB[j][k]);
        }
        tobesorted.push_back(temp);
        grpB.pop_back();
        temp.clear();
    }
    fileContents::iterator itr = tobesorted.begin();
    for (int i = 0; i < tobesortedsize / 2; i++)
    {
        fileContents::iterator itr = tobesorted.begin();
        CSVRow temp;
        for (int k = 0; k <= 3; k++)
        {
            temp.push_back(tobesorted[i][k]);
        }
        grpB.push_back(temp);
        tobesorted.erase(itr + i);
        temp.clear();

    }
    auto engine = default_random_engine{};
    shuffle(begin(tobesorted), end(tobesorted), engine);
    shuffle(begin(grpB), end(grpB), engine);
    write_csv(tobesorted, "NewICSA.csv");
    write_csv(grpB, "NewICSB.csv");
}


/*Function Name: userOutput
*     Variables: int num
*                fstream allstudents
*                        groupa
*                        groupb
*                        male
*                        female
*                char delim
* Reads predefined files and gives the user options to get output
*/
void userOutput()
{
    int num;
    fstream allstudents, groupa, groupb, male, female,oldICSA,oldICSB,oldICSC;
    fileContents allStudents, groupA, groupB, maleS, femaleS,OldICSA,OldICSB,OldICSC;
    char delim = ',';
    readCSV("allstudents.csv", allStudents, delim);
    readCSV("inputICSA.csv", OldICSA, delim);
    readCSV("inputICSB.csv", OldICSB, delim);
    readCSV("inputICSC.csv", OldICSC, delim);
    readCSV("NewICSA.csv", groupA, delim);
    readCSV("NewICSB.csv", groupB, delim);
    readCSV("Malestudents.csv", maleS, delim);
    readCSV("FemaleStudents.csv", femaleS, delim);
    do
    {

        //Gives user options to get output
        cout << "-------------------------------------------------------------------------------";
        cout << "\nEnter a number to represent your output choice\n";
        cout << "A preview shows the first 10 rows and 2 columns\n";
        cout << "0 - To exit the program\n";
        cout << "\n";
        cout << "1 - To preview a table of all the students\n";
        cout << "2 - To preview a table of all the Old ICSA students\n";
        cout << "3 - To preview a table of all the Old ICSB students\n";
        cout << "4 - To preview a table of all the Old ICSC students\n";
        cout << "5 - To preview a table of all the New ICSA students\n";
        cout << "6 - To preview a table of all the New ICSB students\n";
        cout << "7 - To preview a table of all the male students\n";
        cout << "8 - To preview a table of all the female students\n";
        cout << "\n";
        cout << "9  - To view a table of all the students\n";
        cout << "10 - To view a table of all the Old ICSA students\n";
        cout << "11 - To view a table of all the Old ICSB students\n";
        cout << "12 - To view a table of all the Old ICSC students\n";
        cout << "13 - To view a table of all the New ICSA students\n";
        cout << "14 - To view a table of all the New ICSB students\n";
        cout << "15 - To view a table of all the male students\n";
        cout << "16 - To view a table of all the female students\n";
        cout << "\n";
        cout << "17 - To view a custom table of all the students\n";
        cout << "18 - To view a custom table of the Old ICSA students\n";
        cout << "19 - To view a custom table of the Old ICSB students\n";
        cout << "20 - To view a custom table of the Old ICSC students\n";
        cout << "21 - To view a custom table of the New ICSA students\n";
        cout << "22 - To view a custom table of the New ICSB students\n";
        cout << "23 - To view a custom table of the male students\n";
        cout << "24 - To view a custom table of the female students\n";
        cin >> num;
        cout << "-------------------------------------------------------------------------------";

        //Creating instances of the displayGrid for allstudentsGrid
        //groupAGrid,groupBGrid,malesGrid
        displayGrid allStudentsGrid(allStudents, "All Students:");
        displayGrid OldICSAGrid(OldICSA, "inputICSA:");
        displayGrid OldICSBGrid(OldICSB, "inputICSB:");
        displayGrid OldICSCGrid(OldICSC, "inputICSC:");
        displayGrid groupAGrid(groupA, "New ICSA students:");
        displayGrid groupBGrid(groupB, "New ICSB students:");
        displayGrid maleSGrid(maleS, "Male students:");
        displayGrid femaleSGrid(femaleS, "Female students:");

        //Giving user options for output
        switch (num)
        {
        case 1:
            allStudentsGrid.preview();
            break;
        case 2:
            OldICSAGrid.preview();
            break;
        case 3:
            OldICSBGrid.preview();
            break;
        case 4:
            OldICSCGrid.preview();
            break;
        case 5:
            groupAGrid.preview();
            break;
        case 6:
            groupBGrid.preview();
            break;
        case 7:
            maleSGrid.preview();
            break;
        case 8:
            femaleSGrid.preview();
            break;
        case 9:
            allStudentsGrid.printGrid();
            break;
        case 10:
            OldICSAGrid.printGrid();
            break;
        case 11:
            OldICSBGrid.printGrid();
            break;
        case 12:
            OldICSCGrid.printGrid();
            break;
        case 13:
            groupAGrid.printGrid();
            break;
        case 14:
            groupBGrid.printGrid();
            break;
        case 15:
            maleSGrid.printGrid();
            break;
        case 16:
            femaleSGrid.printGrid();
            break;
        case 17:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more than" << allStudents.size() << "\n";
            int allStudentsRows;
            cin >> allStudentsRows;
            if (allStudentsRows <= allStudents.size())
            {
                cout << "\nEnter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int allStudentsCols;
                cin >> allStudentsCols;
                if (allStudentsCols <= 3 && allStudentsCols >= 1)
                {
                    allStudentsGrid.setgridsize(allStudentsRows, allStudentsCols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;
        case 18:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more than" << OldICSA.size() << "\n";
            int OldICSARows;
            cin >> OldICSARows;
            if (OldICSARows <= OldICSA.size())
            {
                cout << "Enter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int OldICSACols;
                cin >> OldICSACols;
                if (OldICSACols <= 3 && OldICSACols >= 1)
                {
                    OldICSBGrid.setgridsize(OldICSARows, OldICSACols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;
        case 19:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more than" << OldICSB.size() << "\n";
            int OldICSBRows;
            cin >> OldICSBRows;
            if (OldICSBRows <= OldICSB.size())
            {
                cout << "Enter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int OldICSBCols;
                cin >> OldICSBCols;
                if (OldICSBCols <= 3 && OldICSBCols >= 1)
                {
                    OldICSBGrid.setgridsize(OldICSBRows, OldICSBCols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;
        case 20:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more than" << OldICSC.size() << "\n";
            int OldICSCRows;
            cin >> OldICSCRows;
            if (OldICSCRows <= OldICSC.size())
            {
                cout << "Enter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int OldICSCCols;
                cin >> OldICSCCols;
                if (OldICSCCols <= 3 && OldICSCCols >= 1)
                {
                    OldICSCGrid.setgridsize(OldICSCRows, OldICSCCols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;
        case 21:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more than" << groupA.size() << "\n";
            int groupARows;
            cin >> groupARows;
            if (groupARows <= groupA.size())
            {
                cout << "Enter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int groupACols;
                cin >> groupACols;
                if (groupACols <= 3 && groupACols >= 1)
                {
                    groupAGrid.setgridsize(groupARows, groupACols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;
        case 22:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more  than" << groupB.size() << "\n";
            int groupBRows;
            cin >> groupBRows;
            if (groupBRows <= groupB.size())
            {
                cout << "Enter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int groupBCols;
                cin >> groupBCols;
                if (groupBCols <= 3 && groupBCols >= 1)
                {
                    groupBGrid.setgridsize(groupBRows, groupBCols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;
        case 23:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more  than" << maleS.size() << "\n";
            int maleSRows;
            cin >> maleSRows;
            if (maleSRows <= maleS.size())
            {
                cout << "Enter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int maleSCols;
                cin >> maleSCols;
                if (maleSCols <= 3 && maleSCols >= 1)
                {
                    maleSGrid.setgridsize(maleSRows, maleSCols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;
        case 24:
            cout << "\nEnter the number of rows you want to display" << "\n";
            cout << "but not more than " << femaleS.size() << "\n";
            int femaleSRows;
            cin >> femaleSRows;
            if (femaleSRows <= femaleS.size())
            {
                cout << "Enter the number of columns you want to display" << "\n";
                cout << "between 1 and 3\n";
                int femaleSCols;
                cin >> femaleSCols;
                if (femaleSCols <= 3 && femaleSCols >= 1)
                {
                    femaleSGrid.setgridsize(femaleSRows, femaleSCols);
                    break;
                }
                else
                    cout << "Kindly try again\n";
                userOutput();
                break;
            }
            else
                cout << "Kindly try again\n";
            userOutput();
            break;

        default:
            cout << "\n\t\t\t\t\tTHANK YOU UNTIL NEXT TIME ! " << "\n";
            break;
        }


    } while (num != 0);
}







