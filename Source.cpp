#define NAME_MAXLENGTH 30
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

struct ResidentInfo
{
    int id;
    //char name[NAME_MAXLENGTH];
    std::string name;
    int days;
    float totalBills;
};

int daysBetweenDates(int startingMonth, int startingDay, int startingYear, int endingMonth, int endingDay, int endingYear)
{
    struct tm date1 = { 0 };
    date1.tm_year = startingYear - 1900;
    date1.tm_mon = startingMonth - 1;
    date1.tm_mday = startingDay;

    struct tm date2 = { 0 };
    date2.tm_year = endingYear - 1900;
    date2.tm_mon = endingMonth - 1;
    date2.tm_mday = endingDay;

    time_t time1 = mktime(&date1);
    time_t time2 = mktime(&date2);
    double difference = difftime(time2, time1);

    int days = difference / (60 * 60 * 24);

    return days;
}

void CalculateTotalBills(float totalBills, int days, std::vector<ResidentInfo> residentInfo)
{
    int totalDays = 0;
    for (unsigned i = 0; i < residentInfo.size(); ++i)
    {
        totalDays += residentInfo[i].days;
    }

    float billsPerDay = totalBills / static_cast<float>(totalDays);
    for (unsigned i = 0; i < residentInfo.size(); ++i)
    {
        residentInfo[i].totalBills = billsPerDay * static_cast<float>(residentInfo[i].days);
        std::cout << "Resident #" << i << ": " << residentInfo[i].name << std::endl;
        std::cout << "Bills are " << residentInfo[i].totalBills << std::endl << std::endl;
    }
}

int main()
{
	std::cout << "Bill Calculator Ver 1.0" << std::endl;
	std::cout << "Written by Sungmin Moon, smlunel@gmail.com" << std::endl;

	float bills;
	std::cout << "Please Enter the total amount of bills: ";
	std::cin >> bills;

	std::cout << "Bills are " << bills << std::endl;

    int startingMonth, startingDay, startingYear, endingMonth, endingDay, endingYear;

    std::cout << "Enter the bill's starting date (MM/DD/YYYY): ";
    std::cin >> std::setw(2) >> startingMonth;
    std::cin.ignore(); // Ignore the '/'
    std::cin >> std::setw(2) >> startingDay;
    std::cin.ignore(); // Ignore the '/'
    std::cin >> std::setw(4) >> startingYear;

    std::cout << "Enter the bill's ending date (MM/DD/YYYY): ";
    std::cin >> std::setw(2) >> endingMonth;
    std::cin.ignore(); // Ignore the '/'
    std::cin >> std::setw(2) >> endingDay;
    std::cin.ignore(); // Ignore the '/'
    std::cin >> std::setw(4) >> endingYear;

    int days = daysBetweenDates(startingMonth, startingDay, startingYear, endingMonth, endingDay, endingYear);

    std::cout << "The number of days between the two dates is: " << days << " days." << std::endl;

    int residentNum;
    std::cout << "Enter how many residents are in the building: ";
    std::cin >> residentNum;
	
    std::vector<ResidentInfo> residentVector;
    residentVector.resize(residentNum);
    for (int i = 0; i < residentNum; i++)
    {
        residentVector[i].id = i;
        std::string name;

        // Ask for resident's name
        std::cout << "Enter the name of resident number #" << i << ": " << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
        std::getline(std::cin, residentVector[i].name); // Use getline to capture full name including spaces

        int residentStartMonth, residentStartDay, residentStartYear;
        int residentEndMonth, residentEndDay, residentEndYear;
        int result = -1;

        // Ask for start date
        std::cout << "Setting up the start date of the resident." << std::endl;
        std::cout << "Enter 0 if you want to set the date automatically to the bill's date." << std::endl;
        std::cout << "Enter 1 if you will enter the date manually: ";
        std::cin >> result;

        if (result == 0)
        {
            // Automatically set start date to the bill's start date
            residentStartMonth = startingMonth;
            residentStartDay = startingDay;
            residentStartYear = startingYear;
        }
        else if (result == 1)
        {
            // Input start date manually
            std::cout << "Enter the bill's starting date (MM/DD/YYYY): ";
            std::cin >> residentStartMonth;
            std::cin.ignore(); // Ignore the '/'
            std::cin >> residentStartDay;
            std::cin.ignore(); // Ignore the '/'
            std::cin >> residentStartYear;
        }

        // Ask for end date
        int result2;
        std::cout << "Setting up the end date of the resident." << std::endl;
        std::cout << "Enter 0 if you want to set the date automatically to the bill's end date." << std::endl;
        std::cout << "Enter 1 if you will enter the date manually: ";
        std::cin >> result2;

        if (result2 == 0)
        {
            // Automatically set end date to the bill's end date
            residentEndMonth = endingMonth;
            residentEndDay = endingDay;
            residentEndYear = endingYear;
        }
        else
        {
            // Input end date manually
            std::cout << "Enter the bill's ending date (MM/DD/YYYY): ";
            std::cin >> residentEndMonth;
            std::cin.ignore(); // Ignore the '/'
            std::cin >> residentEndDay;
            std::cin.ignore(); // Ignore the '/'
            std::cin >> residentEndYear;
        }

        // Calculate the number of days between the start and end dates
        residentVector[i].days = daysBetweenDates(
            residentStartMonth, residentStartDay, residentStartYear,
            residentEndMonth, residentEndDay, residentEndYear
        );
    }
    CalculateTotalBills(bills, days, residentVector);

	return 0;
}