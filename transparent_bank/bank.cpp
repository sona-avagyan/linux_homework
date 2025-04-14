#include <iostream>
#include "bank.h"
#include <climits>
//#include <vector>

Bank::Bank(int n) {
    size = n;    
    for (int ind = 0; ind < size; ind++) {
        bills[ind].cur_balance = 0;
        bills[ind].min_pos_balance = INT_MIN;
        bills[ind].max_pos_balance = INT_MAX;
        bills[ind].is_frozen = false;
        bills[ind].ID = ind;
    }
};

void Bank::print() {
	std::cout << size << std::endl;
	for(int i = 0; i < size; i++)
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
    if (id >= 0 && id < size) { 
       std::cout << "Your current balanse is " << std::endl; 
       return bills[id].cur_balance; 
    } else {
      std::cout << "We can't print your current balance. Try again." << std::endl;
    }
    return -1;
}

int Bank::print_min_balance (int id) {
    if (id >= 0 && id < size) { 
       std::cout << "Your minimal possible balanse is " << std::endl;
       return bills[id].cur_balance;
    } else {
      std::cout << "We can't print your minimal possible balance. Try again." << std::endl;
    }
    return -1;
}

int Bank::print_max_balance (int id) {
    if (id >= 0 && id < size) { 
       std::cout << "Your maximal possible balanse is " << std::endl;
       return bills[id].cur_balance;
    } else {
      std::cout << "We can't print your maximal possible balance. Try again." << std::endl;
    }
    return -1;
}

void Bank::froze (int id) { 
    if (id >= 0 && id < size) {
       bills[id].is_frozen = true;
       std::cout << "Your account have frozen." << std::endl;
    }
}

void Bank::defroze (int id) {
    if (id >= 0 && id < size) {    
       bills[id].is_frozen = false;
       std::cout << "Your account have defrozen." << std::endl;
    }
}

void Bank::transfer (int from_id, int to_id, int sum) {
    if (sum > 0) {
       if (from_id >=0 && from_id < size && to_id >= 0 && to_id < size) {
          if (bills[from_id].cur_balance < bills[from_id].max_pos_balance && bills[from_id].cur_balance > bills[from_id].min_pos_balance) {
             if (!bills[from_id].is_frozen && !bills[to_id].is_frozen) {
                bills[from_id].cur_balance -= sum;
                bills[to_id].cur_balance += sum;
                std::cout << "We transfer " << sum << " dram from an account with " << from_id << "id to account with " << to_id << "id." << std::endl; 
             }
          }
       } 
    }
}


void Bank::enroll_to_all (int sum) {
     if (sum > 0) {
        for (int id = 0; id < size; id++) {
          if (id >=0 && id < size) {
             if (bills[id].cur_balance < bills[id].max_pos_balance && bills[id].cur_balance > bills[id].min_pos_balance) {
                if (!bills[id].is_frozen) {
                   bills[id].cur_balance +=sum;
                   std::cout << "We transfer " << sum << " dram to all accounts." << std::endl;
                }    
             }
          }
        }
     }
}

void Bank::debit_from_all (int sum) {
    if (sum > 0) {
       for (int id = 0; id < size; id++) {
          if (id >=0 && id < size) {
             if (bills[id].cur_balance < bills[id].max_pos_balance && bills[id].cur_balance > bills[id].min_pos_balance) {
                if (!bills[id].is_frozen) {
                   bills[id].cur_balance -=sum;
                   std::cout << "We write off " << sum << " dram from all accounts."<< std::endl;
                }
             }
          }
       }
    }
}


void Bank::set_min_balance (int id, int sum) {
    if (sum > 0) {
      if (id >=0 && id < size) {
         bills[id].min_pos_balance = sum;
         std::cout << "Your minimal possible balance have set to" << bills[id].min_pos_balance << " dram." << std::endl;
      }
   }
}

void Bank::set_max_balance (int id, int sum) {
    if (sum > 0) {  
       if (id >=0 && id < size) {
          bills[id].max_pos_balance = sum;
          std::cout << "Your maximal possible balance have set to" << bills[id].max_pos_balance << " dram." <<std::endl;
       }
    }
}









