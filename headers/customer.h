#ifndef CUSTOMER_H
#define CUSTOMER_H

/*
    This class models a customer that uses the Self Storage facility
    The attributes of the class is in the private section of the class
    The class contains "Room" objects (room.h) in a standard vector
*/

#include <QtCore>
#include <vector>
#include <memory>
#include "room.h"

class Customer {
private:
    // Class Attributes
    QString firstName, lastName, gender, dateJoined;
    int age, customerID, rentalDuration, paymentBalance;
    std::vector<Room> roomsRented; // In database, room seperation - A01|B12|C04
    bool paymentStatus;

public:
    Customer() : firstName{"XXXX"}, lastName{"XXXX"}, gender{"N"}, dateJoined{"0000"}, age{0}, customerID{0}, rentalDuration{0},
    paymentBalance{0}, paymentStatus{false} {}

    Customer(QString aFirst, QString aLast, int aAge, QString aGender, int aCustomerId, QString aDatejoined,
             int aRental, bool aPaymentStatus, int aBalance)
    {
        firstName = aFirst;
        lastName = aLast;
        age = aAge;
        gender = aGender;
        customerID = aCustomerId;
        dateJoined = aDatejoined;
        rentalDuration = aRental;
        paymentStatus = aPaymentStatus;
        paymentBalance = aBalance;
    }

    // Getters
    QString getFullName(){return firstName + " " + lastName;}
    QString getFirstName(){return firstName;}
    QString getLastName(){return lastName;}
    QString getGender(){return gender;}
    QString getDateJoined(){ return dateJoined; }
    int getAge(){return age;}
    int getCustomerID(){ return customerID; }
    int getRentalDuration(){ return rentalDuration; }
    int getPaymentBalance(){ return paymentBalance; }
    bool getPaymentStatus(){ return paymentStatus; }

    std::vector<Room> getCustomerRoomsVector (){ return roomsRented; }
    QString getCustomerRoomsString()
    {
        if (roomsRented.size() == 1){ return roomsRented.at(0).getRoomLabel(); }
        else if (roomsRented.size() == 0) { return "";}
        else {
            QString roomList;
            roomList = roomsRented.at(0).getRoomLabel();
            for (unsigned long long i = 1; i < roomsRented.size(); i++){
                roomList = roomList + "|" + roomsRented.at(i).getRoomLabel();
            }
            return roomList;
        }
    }

    // Modifiers
    void changeName(QString aFirst, QString aLast) {
        firstName = aFirst;
        lastName = aLast;
    }
    void changeAge(int aAge){ age = aAge; }
    void changeGender(QString aGender){ gender = aGender; }
    void addCustomerRooms (Room room){ roomsRented.push_back(room); }
    void removeCustomerRooms(QString label)
    {
        for (unsigned long long i{0}; i < roomsRented.size(); i++)
        {
            if (label == roomsRented.at(i).getRoomLabel())
            {
                roomsRented.erase(roomsRented.begin()+i);
                return;
            }
        }
    }
};

#endif // CUSTOMER_H
