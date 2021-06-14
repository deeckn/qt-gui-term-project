#ifndef SUMMARYSHEET_H
#define SUMMARYSHEET_H

#include <iostream>
#include <vector>
#include "customer.h"
#include "room.h"

class SummarySheet {
private:
    std::vector<Customer> customerList;
    double totalRevenue;
    QString dateCreated;
public:
    SummarySheet() {}
    void calculateTotal();
    void addCustomer(Customer c){customerList.push_back(c);}
};


#endif // SUMMARYSHEET_H
