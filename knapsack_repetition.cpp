// unbounded kanpsack problem (repetition of items allowed)

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>


int max(int a, int b) {  
  return (a > b) ? a : b;
}



// dp with bottom up 
class knapsack_dp_bottomup {
public:
  int run(const std::vector<int>& value, const std::vector<int>& weight,
          const int& capacity, const int& num) {
    
    for (int i = 0; i < capacity+1; ++i) 
       _aux_value.push_back(0);

    for (int i = 1; i < capacity+1; ++i) {
      for (int j = 0; j < num; ++j) {
        if (weight[j] <= i) 
          _aux_value[i] = max(value[j] + _aux_value[i-weight[j]], _aux_value[i]);
      }
    }

    return _aux_value[capacity];
  }

private:
  std::vector<int> _aux_value;
};



int main() {
  std::vector<int> temp;
  std::vector<int> value;
  std::vector<int> weight;
  int capacity;
  int num;
  int sid = 0;

  // read in benchmark
  std::ifstream benchmark("./knapsack_benchmark.txt");
  if (benchmark.is_open()) {
    std::string line;
    std::string v;
    while (getline(benchmark, line)) {
      std::stringstream ss(line);
      while (getline(ss, v, ','))  
        temp.push_back(std::stoi(v));
    }
  }
  benchmark.close();

  
  // in benchmark, 
  // first line represents value of item
  // second line represents weight of item
  // third line represents capacity
  num = temp.size()/2;
  capacity = temp[temp.size()-1];
  for (int i = 0; i < temp.size()/2; ++i)  
    value.push_back(temp[i]);
  for (int i = temp.size()/2; i < temp.size()-1; ++i)  
    weight.push_back(temp[i]);


  // dp with bottom-up version
  knapsack_dp_bottomup dp_bottomup;
  int res = dp_bottomup.run(value, weight, capacity, num);
  std::cout << "by dp bottom-up method , maximum value = " << res << '\n';
  
  return 0;
}
