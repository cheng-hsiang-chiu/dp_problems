// travelling salesman problem

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>



int min(int a, int b) {
  return (a < b) ? a : b;
}



class tsp_naive {
public:
  tsp_naive(const std::vector<std::vector<int>>& distance) : _distance(distance) {
    for(int i = 1; i < _distance.size(); ++i)
      _id.push_back(i);
  }

  int run() {
    int res = std::numeric_limits<int>::max();
    std::vector<int>::iterator it = _id.begin();

    do {
      int tmp = _distance[0][_id[0]];
      
      for(int i = 0; i < _id.size()-1; ++i) 
        tmp += _distance[_id[i]][_id[i+1]];

      tmp += _distance[_id[_id.size()-1]][0];
      
      res = min(res, tmp);
    } while(std::next_permutation(it, it+_id.size()));

    return res;
  }

private:
  std::vector<std::vector<int>> _distance;
  std::vector<int> _id;
};




class tsp_dp_bitmask {
public:
  tsp_dp_bitmask(const std::vector<std::vector<int>>& distance) : _distance(distance) {
    cities = _distance.size();
    std::vector<int> temp(cities, -1);

    for(int i = 0; i < (1 << cities); ++i)
      _aux_matrix.push_back(temp);
  }

  int run(int bitmask, int pos) {
    int res = std::numeric_limits<int>::max();

    //std::cout << "bitmask = " << bitmask << " , pos = " << pos << '\n';

    if(bitmask == ((1 << cities)-1))
      return _distance[pos][0];

    if(_aux_matrix[bitmask][pos] != -1) {
      //std::cout << "HIT : _aux_matrix[" << bitmask << "][" << pos << "] = " << res << '\n'; 
      return _aux_matrix[bitmask][pos];
    }
  
    for(int i = 0; i < cities; ++i) {
      if((bitmask & (1 << i)) == 0)
        res = min(res, run(bitmask | (1 << i), i) + _distance[pos][i]);
    }

    _aux_matrix[bitmask][pos] = res;
    //std::cout << "_aux_matrix[" << bitmask << "][" << pos << "] = " << res << '\n'; 
    return res;
  }

private:
  std::vector<std::vector<int>> _distance;
  std::vector<std::vector<int>> _aux_matrix;
  int cities;
};



int main() {
  std::vector<int> temp;

  int cities = 0;
  
  // read in benchmark
  std::ifstream benchmark("./tsp_benchmark.txt");
  if(benchmark.is_open()) {
    std::string line;
    std::string v;
    while(getline(benchmark, line)) {
      std::stringstream ss(line);
      while(getline(ss, v, ','))
        temp.push_back(std::stoi(v));
    }
  }
  benchmark.close();


  cities = std::sqrt(temp.size());
  std::vector<std::vector<int>> distance(cities, std::vector<int>(cities, 0));

  for(int i = 0; i < cities; ++i) {
    for(int j = 0; j < cities; ++j) {
      distance[i][j] = temp[i*cities+j];
    }
  }


  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;
  
  start = std::chrono::steady_clock::now();
  tsp_naive naive(distance);
  int res = naive.run(); 
  end = std::chrono::steady_clock::now();
  std::cout << "naive method for tsp got result = " << res << " in " \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns.\n";
    
  
  start = std::chrono::steady_clock::now();
  int start_city = 0;
  int bitmask = 1;
  tsp_dp_bitmask dp_bitmask(distance);
  int res1 = dp_bitmask.run(bitmask, start_city1); 
  end = std::chrono::steady_clock::now();
  std::cout << "dp method with bitmask for tsp got result = " << res1 << " in " \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns.\n";
  
  return 0;
}
