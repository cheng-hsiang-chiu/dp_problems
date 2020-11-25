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



class tsp_dp {
public:
  tsp_dp(const std::vector<std::vector<int>> distance) : _distance(distance) {
    for(int i = 0; i < _distance.size(); ++i)
      _visited.push_back(false);
    _num_visited = 1;
  }
  
  int run(const int& start_city) {
    int res = std::numeric_limits<int>::max();
    
    _visited[start_city] = true;

    for(int i = 0; i < _distance.size(); ++i) {
      if(_visited[i] == false) {
        _visited[i] = true;
        ++_num_visited;
        std::cout << "i = " << i << " , _num_visited = " << _num_visited << " , res = " << res << '\n';
        if(_num_visited == _distance.size())
          res = min(res, _distance[start_city][0]);
        else
          res = min(res, run(i) + _distance[start_city][i]);
        _visited[i] = false;
        --_num_visited;
      }
    }

    return res;
  }

private:
  std::vector<bool> _visited;
  int _num_visited;
  std::vector<std::vector<int>> _distance;
};



int main() {
  std::vector<int> temp;

  int cities = 0;
  // read in benchmark
  std::ifstream benchmark("./tsp_benchmark_1.txt");
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
  
  /*
  start = std::chrono::steady_clock::now();
  int start_city = 0;
  tsp_dp dp(distance);
  int res1 = dp.run(start_city); 
  end = std::chrono::steady_clock::now();
  std::cout << "dp method for tsp got result = " << res1 << " in " \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns.\n";
  */

  
  start = std::chrono::steady_clock::now();
  int start_city = 0;
  int bitmask = 1;
  tsp_dp_bitmask dp_bitmask(distance);
  int res2 = dp_bitmask.run(bitmask, start_city); 
  end = std::chrono::steady_clock::now();
  std::cout << "dp method with bitmask for tsp got result = " << res2 << " in " \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns.\n";
  
  return 0;
}
