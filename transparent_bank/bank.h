#pragma once
//#include <vector> 

class Bill {
public:
  int cur_balance;
  int min_pos_balance;
  int max_pos_balance;
  bool is_frozen;
  int ID;
};


class Bank {
private:
   int size;
   Bill bills[];

  
public:
  Bank(int n);
  int print_cur_balance(int id);
  int print_min_balance(int id);
  int print_max_balance(int id);


  void froze(int id);
  void defroze(int id);

  void transfer(int from_id, int to_id, int sum);
  void enroll_to_all(int sum); //add
  void debit_from_all(int sum);  //sub

  void set_min_balance(int id, int sum);
  void set_max_balance(int id, int sum);
  
  void print();
  
  ~Bank();
};

