
#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>


class Solution {

  /**
   * DFS in the graph
   *
   * @param req_map       map of vertex
   * @param result        result obj
   * @param node          current node visited
   * @param already_added bitmap indicating which elements have already
   *                      been added to the result
   * @param bitmap        current historical information of dfs stack to detect
   *                      cyclic dependency
   */
  bool depth_first_search(const std::vector<std::vector<int>>& req_map,
                          std::vector<int>&                    result,
                          int                                  node,
                          std::vector<bool>&                   already_added,
                          std::vector<bool>&                   bitmap)
  {
    if (!already_added[node]) {
        for (const auto& pre : req_map[node]) {
          if (!already_added[pre]) {
            // deadlock by cyclic dependency
            if (bitmap[pre]) {
              return false;
            }
            else {
              // save historic
              bitmap[pre] = true;

              // if one cyclic dependency is detected, propagate it
              if (!depth_first_search(req_map,
                                      result,
                                      pre,
                                      already_added,
                                      bitmap))
              {
                return false;
              }
            }
          }
        }
        // flag it as added to result
        already_added[node] = true;
        result.push_back(node);
    }
    return true;
  }

public:
    std::vector<int>
    findOrder(int                            numCourses,
              std::vector<std::vector<int>>& prerequisites)
    {
      std::vector<std::vector<int>> req_map(numCourses);
      std::vector<bool> already_added(numCourses, false);
      std::vector<int> result;
      result.reserve(numCourses);

      // build adj map
      for (const auto& req_pair : prerequisites) {
          req_map[req_pair[0]].push_back(req_pair[1]);
      }

      for (int ii = 0; ii < numCourses; ii++) {
        // if the node isn't already visited:
        if (!already_added[ii]) {
          std::vector<bool> bitmap(numCourses, false);
          if (!depth_first_search(req_map, result, ii, already_added, bitmap)) {
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