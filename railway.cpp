#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
using namespace std;

class RailwayTicket
{
private:
    int ticketNo;
    char passengerName[50];
    int age;
    char trainName[50];
    char source[30];
    char destination[30];
    int seats;
    double fare;

public:
    void bookTicket(int number)
    {
        ticketNo = number;

        cout << "\nEnter passenger name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.getline(passengerName, 50);

        cout << "Enter age: ";
        cin >> age;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter train name: ";
        cin.getline(trainName, 50);

        cout << "Enter source station: ";
        cin.getline(source, 30);

        cout << "Enter destination station: ";
        cin.getline(destination, 30);

        cout << "Enter number of seats: ";
        cin >> seats;

        cout << "Enter total fare: ";
        cin >> fare;
    }

    void displayTicket() const
    {
        cout << "\n----------------------------------";
        cout << "\nTicket Number : " << ticketNo;
        cout << "\nPassenger Name: " << passengerName;
        cout << "\nAge           : " << age;
        cout << "\nTrain Name    : " << trainName;
        cout << "\nFrom          : " << source;
        cout << "\nTo            : " << destination;
        cout << "\nSeats         : " << seats;
        cout << "\nTotal Fare    : Rs. " << fare;
        cout << "\n----------------------------------\n";
    }

    int getTicketNo() const
    {
        return ticketNo;
    }
};

class RailwaySystem
{
private:
    const char *fileName = "railway.dat";

    int generateTicketNumber()
    {
        ifstream file(fileName, ios::binary);
        RailwayTicket ticket;
        int lastNumber = 1000;

        if (file)
        {
            while (file.read(reinterpret_cast<char*>(&ticket), sizeof(ticket)))
                lastNumber = ticket.getTicketNo();
            file.close();
        }
        return lastNumber + 1;
    }

public:
    void addTicket()
    {
        RailwayTicket ticket;
        int newNumber = generateTicketNumber();

        ticket.bookTicket(newNumber);

        ofstream file(fileName, ios::binary | ios::app);
        if (!file)
        {
            cout << "\nUnable to open ticket file.\n";
            return;
        }

        file.write(reinterpret_cast<char*>(&ticket), sizeof(ticket));
        file.close();

        cout << "\nTicket booked successfully!";
        cout << "\nYour ticket number is: " << newNumber << "\n";
    }

    void viewTickets()
    {
        ifstream file(fileName, ios::binary);
        RailwayTicket ticket;
        bool found = false;

        if (!file)
        {
            cout << "\nNo ticket records found.\n";
            return;
        }

        while (file.read(reinterpret_cast<char*>(&ticket), sizeof(ticket)))
        {
            ticket.displayTicket();
            found = true;
        }

        if (!found)
            cout << "\nNo bookings available.\n";

        file.close();
    }

    void searchTicket()
    {
        int number;
        bool found = false;
        RailwayTicket ticket;

        cout << "\nEnter ticket number to search: ";
        cin >> number;

        ifstream file(fileName, ios::binary);
        if (!file)
        {
            cout << "\nNo ticket records found.\n";
            return;
        }

        while (file.read(reinterpret_cast<char*>(&ticket), sizeof(ticket)))
        {
            if (ticket.getTicketNo() == number)
            {
                cout << "\nTicket found:\n";
                ticket.displayTicket();
                found = true;
                break;
            }
        }

        if (!found)
            cout << "\nTicket number not found.\n";

        file.close();
    }

    void cancelTicket()
    {
        int number;
        bool found = false;
        RailwayTicket ticket;

        cout << "\nEnter ticket number to cancel: ";
        cin >> number;

        ifstream inFile(fileName, ios::binary);
        if (!inFile)
        {
            cout << "\nNo ticket records found.\n";
            return;
        }

        ofstream tempFile("temp.dat", ios::binary);
        while (inFile.read(reinterpret_cast<char*>(&ticket), sizeof(ticket)))
        {
            if (ticket.getTicketNo() == number)
            {
                found = true;
            }
            else
            {
                tempFile.write(reinterpret_cast<char*>(&ticket), sizeof(ticket));
            }
        }

        inFile.close();
        tempFile.close();

        if (found)
        {
            remove(fileName);
            rename("temp.dat", fileName);
            cout << "\nTicket cancelled successfully.\n";
        }
        else
        {
            remove("temp.dat");
            cout << "\nTicket number not found.\n";
        }
    }
};

int main()
{
    RailwaySystem railway;
    int choice;

    do
    {
        cout << "\n========== RAILWAY TICKET MANAGEMENT ==========";
        cout << "\n1. Book Ticket";
        cout << "\n2. View All Tickets";
        cout << "\n3. Search Ticket";
        cout << "\n4. Cancel Ticket";
        cout << "\n5. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            railway.addTicket();
            break;

        case 2:
            railway.viewTickets();
            break;

        case 3:
            railway.searchTicket();
            break;

        case 4:
            railway.cancelTicket();
            break;

        case 5:
            cout << "\nThank you for using the system.\n";
            break;

        default:
            cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}