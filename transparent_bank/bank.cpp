#include <iostream>
#include "bank.h"
#include <climits>
#include <vector>

Bank::Bank(int n) {
    bills.resize(n);
    for (int ind = 0; ind<bills.size(); ind++) {
        bills[ind].cur_balance = 0;
        bills[ind].min_pos_balance = INT_MIN;
        bills[ind].max_pos_balance = INT_MAX;
        bills[ind].is_frozen = false;
        bills[ind].ID = ind;
    }
};

void Bank::print()
{
	std::cout << bills.size() << std::endl;
	std::cout << bills.front().ID << std::endl;
	for(int i = 0; i < bills.size(); i++)
	{
		std::cout << "ID: " << bills[i].ID << "\n\t cur balance: "
			<< bills[i].cur_balance << "\n\t min balance: "
			<< bills[i].min_pos_balance << "\n\t max balance: "
			<< bills[i].max_pos_balance << "\n\t is frozen: "
			<< bills[i].is_frozen << std::endl;
	}
	std::cout << 2 << std::endl;
}

int Bank::print_cur_balance (int id) {
    if (id >= 0 && id < bills.size()) { 
       std::cout << "Your current balanse is " << std::endl; 
       return bills[id].cur_balance; 
    } else {
      std::cout << "We can't print your current balance. Try again" << std::endl; 
    }
}

int Bank::print_min_balance (int id) {
    if (id >= 0 && id < bills.size()) { 
       std::cout << "Your minimal possible balanse is " << std::endl;
       return bills[id].cur_balance;
    } else {
      std::cout << "We can't print your minimal possible balance. Try again" << std::endl;
    }
}

int Bank::print_max_balance (int id) {
    if (id >= 0 && id < bills.size()) { 
       std::cout << "Your maximal possible balanse is " << std::endl;
       return bills[id].cur_balance;
    } else {
      std::cout << "We can't print your maximal possible balance. Try again" << std::endl;
    }
}

void froze (int id) { 
    if (id >= 0 && id < bills.size()) {
       bills[id].is_frozen = true;
    }
}

void defroze (int id) {
    if (id >= 0 && id < bills.size()) {    
       bills[id].is_frozen = false;
    }
}

void transfer (int from_id, int to_id, int sum) {
    if (from_id >=0 && from_id < bills.size() && to_id >= 0 && to_id < bills.size()) {
       if (bills[from_id].cur_balance < bills[from_id].max_pos_balance && bills[from_id].cur_balance > bills[from_id].min_pos_balance) {
          if (!bills[from_id].is_frozen && !bills[to_id].is_frozen) {
              bills[from_id].cur_balance -= sum;
              bills[to_id}.curr_balnce += sum;
          }
       }
    } 
}


void enroll_to_all (int sum) {
    for (int id = 0; id < bills.size(); id++) {
       if (if (id >=0 && id < bills.size()) {
          if (bills[id].cur_balance < bills[id].max_pos_balance && bills[id].cur_balance > bills[id].min_pos_balance) {
             if (!bills[id].is_frozen) {
                bills[id].cur_balance +=sum;
             }    
          }
        }
     }
}

void debit_from_all (int sum) {
    for (int id = 0; id < bills.size(); id++) {
       if (if (id >=0 && id < bills.size()) {
          if (bills[id].cur_balance < bills[id].max_pos_balance && bills[id].cur_balance > bills[id].min_pos_balance) {
             if (!bills[id].is_frozen) {
                bills[id].cur_balance -=sum;
             }
          }
        }
     }
}


void set_min_balance (int id, int sum) {
    if (id >=0 && id < bills.size()) {
       bills[id].min_pos_balance = sum;
    }
}

void set_max_balance (int id, int sum) {
    if (id >=0 && id < bills.size()) {
       bills[id].max_pos_balance = sum;
    }
}









