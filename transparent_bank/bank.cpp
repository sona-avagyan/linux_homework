#include <iostream>
#include "bank.h"
#include <climits>
//#include <vector>

Bank::Bank(int n) {
    size = n;
    bills = new Bill[size];
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    
    for (int ind = 0; ind < size; ind++) {
        pthread_mutex_init(&bills[ind].mutex, &attr);
        bills[ind].cur_balance = 0;
        bills[ind].min_pos_balance = INT_MIN;
        bills[ind].max_pos_balance = INT_MAX;
        bills[ind].is_frozen = false;
        bills[ind].ID = ind;
    }
    pthread_mutexattr_destroy(&attr);
}

Bank::~Bank() {
    for (int i = 0; i < size; ++i) {
        pthread_mutex_destroy(&bills[i].mutex);
    }
    delete[] bills;
}

void Bank::print() {
	//std::cout << size << std::endl;
	for(int i = 0; i < size; i++){
	    pthread_mutex_lock(&bills[i].mutex);	
            std::cout << "ID: " << bills[i].ID << "\n\t cur balance: "
			<< bills[i].cur_balance << "\n\t min balance: "
			<< bills[i].min_pos_balance << "\n\t max balance: "
			<< bills[i].max_pos_balance << "\n\t is frozen: "
			<< bills[i].is_frozen << std::endl;
            pthread_mutex_unlock(&bills[i].mutex);
	}
	//std::cout << 2 << std::endl;
}

//int get_curr_balance(int id) {
   // return bills[id].cur_balance;
//}

void Bank::print_cur_balance (int id) {
    if (id >= 0 && id < size) { 
      pthread_mutex_lock(&bills[id].mutex); 
      std::cout << "Current balanse of id " << id << " is " << bills[id].cur_balance << std::endl; 
      pthread_mutex_unlock(&bills[id].mutex);
    } else {
      std::cout << "We can't print your current balance. Try again." << std::endl;
    }
}

void Bank::print_min_balance (int id) {
    if (id >= 0 && id < size) { 
      pthread_mutex_lock(&bills[id].mutex);
      std::cout << "Minimal possible balanse of id " << id << " is " << bills[id].min_pos_balance;
      pthread_mutex_unlock(&bills[id].mutex);
    } else {
      std::cout << "We can't print your minimal possible balance. Try again." << std::endl;
    }
}

void Bank::print_max_balance (int id) {
    if (id >= 0 && id < size) { 
      pthread_mutex_lock(&bills[id].mutex);
      std::cout <<  "Minimal possible balanse of id " << id << " is " << bills[id].max_pos_balance; 
      pthread_mutex_unlock(&bills[id].mutex);
    } else {
      std::cout << "We can't print your maximal possible balance. Try again." << std::endl;
    }
}

void Bank::froze (int id) { 
    if (id >= 0 && id < size) {
       pthread_mutex_lock(&bills[id].mutex);
       bills[id].is_frozen = true;
       pthread_mutex_unlock(&bills[id].mutex);
       std::cout << "id " << id << "have frozen" << std::endl;
    }
}

void Bank::defroze (int id) {
    if (id >= 0 && id < size) {    
       pthread_mutex_lock(&bills[id].mutex);
       bills[id].is_frozen = false;
       pthread_mutex_unlock(&bills[id].mutex);
       std::cout << "id " << id << "have defrozen" << std::endl;
    }
}

void Bank::transfer (int from_id, int to_id, int sum) {
    if (sum > 0) {
       if (from_id >=0 && from_id < size && to_id >= 0 && to_id < size) {
          int first = std::min(from_id, to_id);
          int second = std::max(from_id, to_id);

          pthread_mutex_lock(&bills[first].mutex);
          pthread_mutex_lock(&bills[second].mutex);

          if (bills[from_id].cur_balance < bills[from_id].max_pos_balance && bills[from_id].cur_balance > bills[from_id].min_pos_balance) {
             if (!bills[from_id].is_frozen && !bills[to_id].is_frozen) {
                bills[from_id].cur_balance -= sum;
                bills[to_id].cur_balance += sum;
                std::cout << sum << " dram tranfered from an account with " << from_id << "id to account with " << to_id << "id." << std::endl; 
             }
          }
         pthread_mutex_unlock(&bills[second].mutex);
         pthread_mutex_unlock(&bills[first].mutex);
       }
    }
}


void Bank::enroll_to_all (int sum) {
     if (sum > 0) {
        for (int id = 0; id < size; id++) {
           pthread_mutex_lock(&bills[id].mutex);
           if (id >=0 && id < size) {
             if (bills[id].cur_balance < bills[id].max_pos_balance && bills[id].cur_balance > bills[id].min_pos_balance) {
                if (!bills[id].is_frozen) {
                   bills[id].cur_balance +=sum;
                   std::cout << sum << " dram transfered to all accounts." << std::endl;
                }    
             }
           }
           pthread_mutex_unlock(&bills[id].mutex);
        }
     }
}

void Bank::debit_from_all (int sum) {
    if (sum > 0) {
       for (int id = 0; id < size; id++) {
          pthread_mutex_lock(&bills[id].mutex);
          if (id >=0 && id < size) {
             if (bills[id].cur_balance < bills[id].max_pos_balance && bills[id].cur_balance > bills[id].min_pos_balance) {
                if (!bills[id].is_frozen) {
                   bills[id].cur_balance -=sum;
                   std::cout << sum << " dram transfered from all accounts."<< std::endl;
                }
             }
          }
          pthread_mutex_unlock(&bills[id].mutex);
       }
    }
}


void Bank::set_min_balance (int id, int sum) {
    if (sum > 0) {
      if (id >=0 && id < size) {
         pthread_mutex_lock(&bills[id].mutex);
         bills[id].min_pos_balance = sum;
         pthread_mutex_unlock(&bills[id].mutex);
         std::cout << "Minimal possible balance of id" << id << "set to" << bills[id].min_pos_balance << " dram." << std::endl;
      }
   }
}

void Bank::set_max_balance (int id, int sum) {
    if (sum > 0) {  
       if (id >=0 && id < size) {
          pthread_mutex_lock(&bills[id].mutex);
          bills[id].max_pos_balance = sum;
          pthread_mutex_unlock(&bills[id].mutex);
          std::cout << "Maximal possible balance of id" << id << "set to" << bills[id].max_pos_balance << " dram." << std::endl;
       }
    }
}

