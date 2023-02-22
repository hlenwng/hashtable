/*
  Name: Helen Wang
  Date: Feb 20, 2023
  Program: Add students (manually or randomly generated) and store them in a linked table. 
 */

#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstring>
#include <time.h>
using namespace std;

struct Student {
  char* fname = new char[20];
  char* lname = new char[20];
  int id;
  float gpa;
  int hash;
  Student* next = NULL;
public:
  void printStu() {
    cout << fname << ", " << lname << ", " << id << ", " << gpa << endl;
  }
};

Student** add(Student* student, Student** table, int size);
Student* generate();
void print(Student** table, int size);
int hashFunction(Student* student, int size);
Student** rehash(Student** table, int size);
void deletee(Student** table, int size);

int main() {
  srand(time(NULL));
  int size = 100;
  Student** table = new Student*[size];
  
  bool playing = true;
  char command[15];
  char ADD[] = "ADD";
  char PRINT[] = "PRINT";
  char DELETE[] = "DELETE";
  char GENERATE[] = "GENERATE";
  char QUIT[] = "QUIT";
  bool Rehash = false;

  while (playing == true) {

    cout << endl;
    //User chooses to add, print, delete or quit
    cout << "ADD, PRINT, DELETE or QUIT" << endl;
    cin.get(command, 15, '\n');
    cin.ignore();

    if (strcmp(command, "add") == false) {
      cout << "Add manually or generate?" << endl;
      cin.get(command, 15, '\n');
      cin.ignore();

      //Generate student(s)
      if (strcmp(command, "generate") == false) {
	//Find out how many students to generate                            
	cout << "How many students do you want to generate?" << endl;
	int num = 0;
	cin >> num;
	cin.ignore();
	
	for (int i = 0; i < num; i++) {
	  Student* newStu = generate();
	  
	  table = add(newStu, table, size);
	  
	  int collisions = 1;
	  //Check for collisions in table
	  for (int i = 0; i < size && collisions < 3; i++) {
	    collisions = 0;
	    Student* current = table[i];
	    if (current!=NULL) {
	      while (current!=NULL) {
		collisions++;
		
		current = current->next;
	      }
	    }
	    
	    if (collisions > 3) {
	      size = size*2;
	      table = rehash(table, size);                              
	    }
	  }
	}	
      }

      //Add student manually
      else if (strcmp(command, "manually") == false){

	Student* student = new Student();	
	cout << "first name: ";
	cin >> student->fname;
	
	cout << "last name: ";
	cin >> student->lname;
	
	cout << "id #: ";
	cin >> student->id;
	
	cout << "gpa: ";
	cin >> student->gpa;
	cin.ignore();
	student->printStu();
	table = add(student, table, size);
      }
    }
    
    else if (strcmp(command, "print") == false) {
      print(table, size);
    }
    
    else if (strcmp(command, "delete") == false) {
      deletee(table, size);
    }

    else if (strcmp(command, "quit") == false) {
      playing = false;
    }

    else {
      cout << "Please enter a valid command" << endl;
      cout << "Or, type in lowercase" << endl;
    }
  }
}

//Add new student
Student** add(Student* student, Student** table, int size) {
  Student* current = table[student->hash];
  int collisions = 1;

  //Check and rehash if needed
  if (current == NULL) {
    table[student->hash] = student;
  }
  else if (table[student->hash] != NULL) {
    while (current->next !=NULL) {
      current = current->next;
      collisions++;
    }
    current->next = student;
  }
  //  cout << "Student added!" << endl;
  return table;  
}

//Generate random students
Student* generate() {

  int numLine = 0;

  //Read in a random line from txt files
  //Take random first name
  Student* student = new Student();
  int fRandom = rand()%100;
  ifstream ffile;
  ffile.open("fname.txt");
  
  char fvalue[30];
  //Go till random generated line #
  while (fRandom!=numLine && ffile.getline(fvalue, sizeof(fvalue))) {
    ++numLine;
  }
  if (numLine == fRandom) {
    strcpy(student->fname, fvalue);
  }
  
  ffile.close();

  //Take random last name
  int lRandom = rand()%100;
  ifstream lfile;
  lfile.open("lname.txt");
  char lvalue[30];
  while (lRandom!=numLine && lfile.getline(lvalue, sizeof(lvalue))) {
    ++numLine;
  }
  if (numLine == lRandom) {
    strcpy(student->lname, lvalue);
  }
  lfile.close();

  //Randomly generate id # and gpa
  int stuid = rand()%100002;
  student->id = stuid;
  float g = rand()&125;
  float stugpa = (double)(rand()%41)/10;
  student->gpa = stugpa;

  student->printStu();
  return student;
}

void print (Student** table, int size) {
  cout << endl;

  //Go through table and print out values
  for (int i = 0; i < size; i++) {
    if (table[i] != NULL) {
      cout << "First name: " << table[i]->fname << endl;
      cout << "Last name: " << table[i]->lname << endl;
      cout << "ID: " << table[i]->id << endl;
      cout.setf(ios::fixed, ios::floatfield);
      cout.setf(ios::showpoint);
      cout.precision(2);
      cout << "GPA: " << table[i]->gpa << endl;
      cout << endl;

      Student* current = table[i];

      while(current->next != NULL) {
	current = current->next;
	cout << "First name: "	<< current->fname << endl;
	cout << "Last name: " << current->lname << endl;
	cout << "ID: "	<< current->id	<< endl;
	cout.setf(ios::fixed, ios::floatfield);
	cout.setf(ios::showpoint);
	cout.precision(2);
	cout << "GPA: " << current->gpa << endl;
	cout << endl;
      }
    }    
  }
}

//Function to hash students
int hashFunction(Student* student, int size) {
  return student->hash = student->id % size;
}

//Function to rehash table
Student** rehash(Student** table, int size) {
  int count = 0;

  //Create new table that is double the size
  Student** newTable = new Student*[size*2];
  //Clear values of new table
  for (int i = 0; i < size*2; i++) {
    newTable[i] = NULL;
  }

  //Rehash old table->new table
  for (int i = 0; i < size; i++) {
    Student* temp = table[i];
    while (temp != NULL) {
      temp->hash = hashFunction(temp, size);
      Student* newS = new Student();
      //Copy over student
      newS->next = NULL;
      newS->fname = temp->fname;
      newS->lname = temp->lname;
      newS->id = temp->id;
      newS->gpa = temp->gpa;
      
      table = add(newS, table, size);
      newS = newS->next;
    }
  }
  cout << "rehashed!" << endl;
}

void deletee(Student** table, int size) {
  int sid = 0;

  cout << "Which student (ID) do you want to delete?" << endl;
  cin >> sid;
  cin.ignore();

  //Go through till student is found
  //Delete student and move other students up a slot
  for (int i = 0; i < size; i++) {
    if (table[i] != NULL) {
      Student* temp = table[i];

      if (temp->next == NULL) {
	if (temp->id == sid) {
	  table[i] = NULL;
	}
      }

      else if (table[i]->id == sid) {
	table[i] = table[i]->next;
      }
      else {
	Student* temp2;
	while (temp->id != sid) {
	  temp2 = temp;
	  temp = temp->next;
	}

	temp2->next = temp->next;
      }
    }  
  }
  cout << "Student ID: " << sid << " has been deleted!" << endl;
}
