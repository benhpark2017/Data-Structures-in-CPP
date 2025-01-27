/**
 * Solutions to Chapter 7, Section 9, Exercise 7 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.* 
 * 
 * Task 7:
 * Assume you are working in the census department of a small town where the
 * number of records, about 3000, is small enough to fit into the internal
 * memory of a computer. All the people currently living in this town were born 
 * in the United States. There is one record for each person in this town. Each 
 * record contains (a) the state in which the person was born, (b) county of 
 * birth, and (c) name of person. In C++, how would you produce a list of all 
 * persons living in this town? The list is to be ordered by state. Within each 
 * state the persons are to be listed by their counties, the counties being 
 * arranged in alphabetical order. Within each county, the names are also listed 
 * in alphabetical order. Justify any assumptions you make.
 * 
 * Assumptions:
 * 1. Data Fits in Memory:
 * The number of records (approximately 3000) is small enough to fit into the 
 * computer's internal memory. This assumption allows us to use in-memory data 
 * structures like std::vector in C++ without worrying about external storage 
 * or memory limitations.
 * 
 * 2. Single Record per Person:
 * There is exactly one record for each person in the town. This means no 
 * duplicate records or multiple entries for the same person exist in the 
 * data set.
 * 
 * 3. All People Were Born in the United States:
 * The problem specifies that all individuals were born in the United States. 
 * This assumption simplifies the data model, as we don't need to account for
 * international birthplaces or additional fields like country of birth.
 * 
 * 4. Names Are in "First Name Last Name" Format:
 * The name field in each record is assumed to be in the format
 * "First Name Last Name" (e.g., "John Doe"). This allows us to split the name 
 * into firstName and lastName for sorting purposes. If names were in a 
 * different format (e.g., "Last Name, First Name"), additional parsing logic 
 * would be required.
 * 
 * 5. No Missing Data:
 * All records are assumed to have complete data. There are no missing values 
 * for state, county, or name. If missing data were possible, additional logic 
 * would be needed to handle such cases (e.g., default values or filtering out 
 * incomplete records).
 * 
 * 6. Sorting Order:
 * The desired sorting order is clearly defined:
 * First by state (alphabetical order).
 * Within the same state, by county (alphabetical order).
 * Within the same county, by lastName (alphabetical order).
 * If lastName is the same, by firstName (alphabetical order).
 * 
 * 7. Encapsulation and Object-Oriented Design:
 * The Person class is designed with encapsulation in mind, meaning the internal 
 * data (state, county, firstName, lastName) is private and accessed only
 * through public getter methods. This assumption ensures data integrity and 
 * adheres to object-oriented principles.
 * 
 * 8. Sorting Algorithm Choice:
 * The choice of sorting algorithm (e.g., Merge Sort or Quick Sort) is based on
 * the assumption that the dataset is small enough for these algorithms to 
 * perform efficiently. For larger datasets, more advanced techniques (e.g., 
 * external sorting) might be required.
 * 
 * 9. No Special Characters or Localization:
 * The names, states, and counties are assumed to contain only standard
 * alphanumeric characters. There are no special characters, accents, or 
 * localization considerations (e.g., sorting names in non-English languages).
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // For splitting names

class Person {
private:
    std::string state;
    std::string county;
    std::string firstName;
    std::string lastName;

public:
    // Constructor
    Person(const std::string& state, const std::string& county, const std::string& fullName) {
        this->state = state;
        this->county = county;
        // Split the full name into first and last names
        std::istringstream iss(fullName);
        iss >> firstName >> lastName;
    }

    // Getters
    std::string getState() const { return state; }
    std::string getCounty() const { return county; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getFullName() const { return firstName + " " + lastName; }

    // Overload the < operator for sorting
    bool operator<(const Person& other) const {
        if (state != other.state) {
            return state < other.state; // Sort by state
        } else if (county != other.county) {
            return county < other.county; // Sort by county within the same state
        } else if (lastName != other.lastName) {
            return lastName < other.lastName; // Sort by last name within the same county
        } else {
            return firstName < other.firstName; // Sort by first name if last names are the same
        }
    }
};

// Partition function for Quick Sort
int partition(std::vector<Person>& persons, int low, int high) {
    Person pivot = persons[high]; // Choose the last element as the pivot
    int i = low - 1; // Index of the smaller element

    for (int j = low; j < high; j++) {
        if (persons[j] < pivot) {
            i++; // Increment the index of the smaller element
            std::swap(persons[i], persons[j]);
        }
    }
    std::swap(persons[i + 1], persons[high]); // Place the pivot in the correct position
    return i + 1; // Return the partition index
}

// Quick Sort function
void quickSort(std::vector<Person>& persons, int low, int high) {
    if (low < high) {
        // Partition the array and get the pivot index
        int pivot = partition(persons, low, high);

        // Recursively sort the subarrays
        quickSort(persons, low, pivot - 1); // Sort the left subarray
        quickSort(persons, pivot + 1, high); // Sort the right subarray
    }
}

int main() {
    // Expanded test data
    std::vector<Person> persons = {
        {"California", "Los Angeles", "John Doe"},
        {"Texas", "Harris", "Jane Smith"},
        {"California", "San Francisco", "Alice Johnson"},
        {"Texas", "Dallas", "Bob Brown"},
        {"California", "Los Angeles", "Charlie Davis"},
        {"New York", "New York", "Michael Taylor"},
        {"Florida", "Miami-Dade", "Emily White"},
        {"Illinois", "Cook", "David Wilson"},
        {"Ohio", "Franklin", "Linda Martinez"},
        {"Georgia", "Fulton", "James Anderson"},
        {"California", "San Diego", "Sophia Lee"},
        {"Texas", "Travis", "William Garcia"},
        {"New York", "Bronx", "Emma Harris"},
        {"Florida", "Broward", "Daniel Clark"},
        {"Illinois", "DuPage", "Olivia Lewis"},
        {"Ohio", "Cuyahoga", "Ethan Walker"},
        {"Georgia", "Gwinnett", "Ava Hall"},
        {"California", "Orange", "Mason Young"},
        {"Texas", "Bexar", "Isabella Allen"},
        {"New York", "Queens", "Noah King"},
        {"Florida", "Palm Beach", "Charlotte Scott"},
        {"Illinois", "Lake", "Liam Green"},
        {"Ohio", "Hamilton", "Amelia Adams"},
        {"Georgia", "Cobb", "Benjamin Nelson"},
        {"California", "Santa Clara", "Mia Carter"},
        {"Texas", "Collin", "Elijah Mitchell"},
        {"New York", "Bronx", "Allison Roberts"},
        {"Florida", "Hillsborough", "Logan Perez"},
        {"Illinois", "Will", "Evelyn Turner"},
        {"Ohio", "Summit", "Alexander Phillips"},
        {"Georgia", "DeKalb", "Sofia Campbell"},
        {"California", "Alameda", "Daniel Parker"},
        {"Texas", "Denton", "Grace Evans"},
        {"New York", "Richmond", "Jackson Edwards"},
        {"Florida", "Pinellas", "Avery Collins"},
        {"Illinois", "Kane", "Abigail Stewart"},
        {"Ohio", "Montgomery", "Ryan Sanchez"},
        {"Georgia", "Chatham", "Ella Morris"},
        {"California", "Contra Costa", "Carter Rogers"},
        {"Texas", "El Paso", "Scarlett Reed"},
        {"New York", "Nassau", "Luke Bailey"},
        {"Florida", "Orange", "Chloe Murphy"},
        {"Illinois", "McHenry", "Jack Rivera"},
        {"Ohio", "Lucas", "Lily Cook"},
        {"Georgia", "Clayton", "Jayden Bell"}
    };

    // Perform Quick Sort
    quickSort(persons, 0, persons.size() - 1);

    // Output the sorted list
    for (const auto& person : persons) {
        std::cout << "State: " << person.getState()
                  << ", County: " << person.getCounty()
                  << ", Name: " << person.getFullName() << std::endl;
    }

    return 0;
}
