#include <iostream>
#include <string>
#include <vector>
#include "json.hpp"
#include "httplib.h"
#include <random>

using namespace std;
using namespace httplib;
using namespace nlohmann;

const vector<string> ACTIONS = { "move", "eat", "load", "unload" };
const vector<string> DIRECTIONS = { "up", "down", "right", "left" };

int main(int argc, char* argv[])
{
  // Initialize random number generator
  random_device rd;
  mt19937 mt(rd());
  uniform_int_distribution<size_t> dist1(0, ACTIONS.size()-1);
  uniform_int_distribution<size_t> dist2(0, DIRECTIONS.size()-1);

  // Initialize the http server
  Server svr;
  svr.Post("/", [&](const Request& req, Response& res) {
    // Hive object from request payload
    auto hive = json::parse(req.body);

    // Loop through ants and give orders
    json orders;
    for (auto& ant : hive["ants"].items()) {
      orders[ant.key()] = {
        {"act", ACTIONS[dist1(mt)] },
        {"dir", DIRECTIONS[dist2(mt)]}
      };
    }

    cout << orders.dump() << '\n';
    res.set_content(orders.dump(), "application/json");
    // json format sample:
    // {"1":{"act":"load","dir":"down"},"17":{"act":"load","dir":"up"}}
  });

  svr.listen("0.0.0.0", 7070);
  return 0;
}
