# Employee Management System

## Description

The **Employee Management System** is a console-based application written in **C** that helps manage a list of employees. This system provides features such as adding, deleting, modifying, searching, printing, and saving employee records. It uses a `struct` to represent each employee, storing information like:
- Employee ID
- Name
- Salary
- Date of Birth
- Address
- Phone Number
- Employment Date
- Email

This program is designed to be simple yet effective for basic employee management tasks.

---

## Features

1. **Loading Existing Data**: 
   - On startup, the program prompts for a file name to load employee data from a file.
   
2. **Adding an Employee**:
   - The user can add one or more employees by providing details such as ID, name, salary, birth date, etc.
   
3. **Deleting an Employee**:
   - The user can delete an employee by entering their employee ID.
   
4. **Modifying Employee Details**:
   - The user can modify specific details for any employee by entering the employee ID.
   
5. **Searching for an Employee**:
   - The user can search for an employee by their name.
   
6. **Printing Employee Records**:
   - The user can print all employee records sorted by either name, salary, or date of birth.
   
7. **Saving Employees to File**:
   - The user can save the current employee records to a file.
   
8. **Quit Program**:
   - The program can be safely quit, with a confirmation prompt to prevent accidental exits.

---

## How to Use

### 1. Loading Existing Data
- When the program starts, you will be prompted to enter the name of the file containing the employee data. If the file exists, it will load the data into the system.

### 2. Adding an Employee
- Select **Option 1** from the main menu to add an employee.
- Enter the number of employees you wish to add.
- Provide the required details (ID, name, salary, birth date, etc.) for each employee.

### 3. Deleting an Employee
- Select **Option 2** from the main menu.
- Enter the ID of the employee you wish to delete.

### 4. Modifying Employee Details
- Select **Option 3** from the main menu.
- Enter the ID of the employee you wish to modify.
- Choose the field you want to update (name, salary, phone number, etc.) and provide the new value.

### 5. Searching for an Employee
- Select **Option 4** from the main menu.
- Enter the name of the employee you're searching for.

### 6. Printing Employee Records
- Select **Option 5** from the main menu.
- Choose how you'd like to sort the records ('N' for name, 'S' for salary, 'D' for date of birth).
- The program will display the sorted records on the screen.

### 7. Saving Employee Records to a File
- Select **Option 6** from the main menu.
- Enter the filename to save the employee data. The program will overwrite the existing file or create a new one if it doesn't exist.

### 8. Quitting the Program
- Select **Option 7** from the main menu to quit.
- The program will ask for confirmation to ensure no unsaved work is lost.

---

## Algorithms Used

### Search Algorithm
- The program uses a simple **linear search algorithm** to find an employee based on their name.

### Sort Algorithms
1. **Sort By Name**:
   - Uses the **Bubble Sort** algorithm to sort employees alphabetically by name.
   
2. **Sort By Salary**:
   - Uses the **Bubble Sort** algorithm to sort employees in descending order by salary.

3. **Sort By Date of Birth**:
   - Uses the **Bubble Sort** algorithm to sort employees in ascending order by date of birth.
