
#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>


class Solution {

  bool deep_search(std::vector<int>&  result,
                   int                node,
                   std::vector<bool>& already_added,
                   std::vector<bool>& bitmap)
  {
    if (!already_added[node]) {
        for (const auto& pre : prereq[node]) {
          if (!already_added[pre]) {
            // deadlock by ciclic dependency
            if (bitmap[pre]) {
              return false;
            }
            else {
              bitmap[pre] = true;
              if (!deep_search(result, pre, already_added, bitmap)) {
                return false;
              }
            }
          }
        }
        already_added[node] = true;
        result.push_back(node);
    }
    return true;
  }

  std::unordered_map<int, std::vector<int>> prereq;
public:
    std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
      std::vector<int> result;
      result.reserve(numCourses);

      for (const auto& req_pair : prerequisites) {
          prereq[req_pair[0]].push_back(req_pair[1]);
      }

      std::vector<bool> already_added(numCourses, false);
      std::vector<int> temp;
      for (int ii = 0; ii < numCourses; ii++) {
        if (!already_added[ii]) {
          std::vector<bool> bitmap(numCourses, false);
          if (!deep_search(result, ii, already_added, bitmap)) {
            result.clear();
            break;
          }
        }
      }
      return result;
    }
};

using namespace std;
int main() {
  vector<vector<int>> prereq;
  string line;
  int total = 0;
  Solution solve;
  cin >> total;
  cin.ignore();
  getline(cin, line);
  auto it = line.find("[");
  if (it != string::npos) {
    stringstream ss(line);
    ss.ignore();
    while (ss.good() && !ss.eof()){
      int req, node;
      ss.ignore(); //[
      ss >> req;
      ss.ignore(); //,
      ss >> node;
      if (ss.good()) {
        prereq.push_back({node, req});
        ss.ignore(); //]
        ss.ignore(); //,or]
      }
    }
  }

  stringstream ss;
  ss << "[";
  for (const auto &ans : solve.findOrder(total, prereq)) {
    ss << ans << ",";
  }
  auto str = ss.str();
  if (str.back() == ',') {
    str.pop_back();
  }
  str.push_back(']');

  cout << str;
}