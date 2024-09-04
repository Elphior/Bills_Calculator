#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct ResidentInfo
{
    int id;
    std::string name;
    int days;
    float totalBills;
};

struct Date
{
    int month;
    int day;
    int year;
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
        std::cout << residentInfo[i].days << " days of utility usage" << std::endl;
        std::cout << "Bills are " << residentInfo[i].totalBills << std::endl << std::endl;
    }
}

void parseData(const std::string& dateString, int& month, int& day, int& year)
{
    std::stringstream ss(dateString);
    char separator;
    ss >> month >> separator >> day >> separator >> year;
}

bool isBefore(const Date& d1, const Date& d2)
{
    if (d1.year < d2.year)   return true;
    if (d1.year > d2.year)   return false;
    if (d1.month < d2.month)   return true;
    if (d1.month > d2.month)   return false;
    return d1.day < d2.day;
}

bool isAfter(const Date& d1, const Date& d2)
{
    return !isBefore(d1, d2);
}


bool ResidentSetup(std::vector<ResidentInfo>& residentVector, const Date& billingStart, const Date& billingEnd)
{
    std::ifstream file("resident.csv");
    if (!file.is_open())
    {
        return false;
    }
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string idString, nameString, moveInDateString, moveOutDateString;

        std::getline(ss, idString, ',');
        std::getline(ss, nameString, ',');
        std::getline(ss, moveInDateString, ',');
        std::getline(ss, moveOutDateString, ',');

        int startMonth, startDay, startYear;
        int endMonth, endDay, endYear;
        parseData(moveInDateString, startMonth, startDay, startYear);
        parseData(moveOutDateString, endMonth, endDay, endYear);

        // Check if Move-in or Move-out dates are outside of bill date range.
        Date moveInDate;
        moveInDate.month = startMonth;
        moveInDate.day = startDay;
        moveInDate.year = startYear;

        Date moveOutDate;
        moveOutDate.month = endMonth;
        moveOutDate.day = endDay;
        moveOutDate.year = endYear;

        int days;
        if (isBefore(moveOutDate, billingStart) || isAfter(moveInDate, billingEnd))
            days = 0;
        else
        {
            if (isBefore(moveInDate, billingStart))
                moveInDate = billingStart;
            if (isAfter(moveOutDate, billingEnd))
                moveOutDate = billingEnd;

            //days = daysBetweenDates(startMonth, startDay, startYear, endMonth, endDay, endYear);
            days = daysBetweenDates(moveInDate.month, moveInDate.day, moveInDate.year,
                                    moveOutDate.month, moveOutDate.day, moveOutDate.year);
        }
        ResidentInfo resident;
        resident.id = std::stoi(idString);
        resident.name = nameString;
        resident.days = days;

        residentVector.push_back(resident);
    }

    file.close();
    return true;
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

    Date billingStart;
    billingStart.month = startingMonth;
    billingStart.day = startingDay;
    billingStart.year = startingYear;

    Date billingEnd;
    billingEnd.month = endingMonth;
    billingEnd.day = endingDay;
    billingEnd.year = endingYear;

    std::vector<ResidentInfo> residentVector;
    ResidentSetup(residentVector, billingStart, billingEnd);

    CalculateTotalBills(bills, days, residentVector);

	return 0;
}