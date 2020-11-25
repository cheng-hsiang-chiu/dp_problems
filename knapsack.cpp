// 0-1 kanpsack problem

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>



int max(int a, int b) {  
  return (a > b) ? a : b;
}



// dp with bottom up 
class knapsack_dp_bottomup {
public:
  int run(const std::vector<int>& value, const std::vector<int>& weight,
          const int& capacity, const int& num) {
    
    std::vector<int> temp(capacity+1, 0);
    for (int i = 0; i < num+1; ++i) 
       _aux_value.push_back(temp);

    for (int i = 1; i < num+1; ++i) {
      for (int j = 1; j < capacity+1; ++j) {
        if (weight[i-1] > j) 
          _aux_value[i][j] = _aux_value[i-1][j];
        
        else 
          _aux_value[i][j] = max(value[i-1] + _aux_value[i-1][j-weight[i-1]],
                                 _aux_value[i-1][j]);
      }
    }

    return _aux_value[num][capacity];
  }

private:
  std::vector<std::vector<int>> _aux_value;
};


// dp with memorization version
class knapsack_dp {
public:
  int run(const std::vector<int>& value, const std::vector<int>& weight,
          int capacity, const int& num, int sid) {
    
    std::vector<int> temp(capacity+1, -1);
    for (int i = 0; i < num; ++i) 
      _aux_value.push_back(temp);
    
    return run_helper(value, weight, capacity, num, sid);
  }

  int run_helper(const std::vector<int>& value, const std::vector<int>& weight,
                 int capacity, const int& num, int sid) {

    if (!capacity || !num)  return 0;

    if (sid >= num)  return 0;

    if (_aux_value[sid][capacity] != -1)  return _aux_value[sid][capacity]; 
    
    if (weight[sid] > capacity) {
      _aux_value[sid][capacity] = run_helper(value, weight, capacity, num, sid+1);
      return _aux_value[sid][capacity];
    }
    else {
      _aux_value[sid][capacity] = max(value[sid] + run_helper(value, weight, capacity-weight[sid], num, sid+1),
                                      run_helper(value, weight, capacity, num, sid+1));
      return _aux_value[sid][capacity];
    }
  }
private:
  std::vector<std::vector<int>> _aux_value;
};



// recursive version
class knapsack_recursive {
public:
  int run(const std::vector<int>& value, const std::vector<int>& weight, 
          int capacity, const int& num, int sid) {

    if (!capacity || !num)  return 0;

    if (sid >= num)  return 0;

    if (weight[sid] > capacity)
      return run(value, weight, capacity, num, sid+1);
    else
      return max(value[sid] + run(value, weight, capacity-weight[sid], num, sid+1), 
                 run(value, weight, capacity, num, sid+1));
  }
};


int main() {
  std::vector<int> temp;
  std::vector<int> value;
  std::vector<int> weight;
  int capacity;
  int num;
  int sid = 0;

  // read in benchmark
  std::ifstream benchmark("./knapsack_benchmark_1.txt");
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

 

  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;


  
  // recursive version
  start = std::chrono::steady_clock::now();
  knapsack_recursive recursive;
  int res = recursive.run(value, weight, capacity, num, sid);
  end = std::chrono::steady_clock::now();
  std::cout << "by naive method, maximum value = " << res \
            << " in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns\n";
  

  // dp with top-down version
  start = std::chrono::steady_clock::now();
  knapsack_dp dp;
  int res1 = dp.run(value, weight, capacity, num, sid);
  end = std::chrono::steady_clock::now();
  std::cout << "by dp top-down method , maximum value = " << res1 \
            << " in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns\n";

  // dp with bottom-up version
  start = std::chrono::steady_clock::now();
  knapsack_dp_bottomup dp_bottomup;
  int res2 = dp_bottomup.run(value, weight, capacity, num);
  end = std::chrono::steady_clock::now();
  std::cout << "by dp bottom-up method , maximum value = " << res2 \
            << " in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns\n";
  
  return 0;
}
