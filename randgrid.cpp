#include <iostream>
#include <string>
#include <set>
#include <random>
#include "tinyformat/tinyformat.h"
#include "lain/algorithms.h"
#include "lain/argparse.h"
#include "lain/string.h"
#include "lain/settings.h"

namespace tfm = tinyformat;

class Point {
public:
   int x;
   int y;

   Point(const int x, const int y) {
      this->x = x;
      this->y = y;
   }

   friend bool operator<(const Point& a,const Point& b) {
      return a.x == b.x ? a.y < b.y : a.x < b.x;
   }
};

std::set<Point> read_from_log(const lain::Settings& log) {
   std::set<Point> points;
   for (auto& json : log.get_object_array("points")) {
      points.insert(Point(json.get<int>("x"), json.get<int>("y"))); 
   }
   return points;
}

lain::Settings write_to_log(const std::set<Point> points) {
   lain::Settings log;
   std::vector<lain::Settings> json_list;
   for (auto& pt : points) {
      lain::Settings json;
      json.set<double>("x", pt.x);
      json.set<double>("y", pt.y);
      json_list.push_back(json);
   }
   log.set_object_array("points", json_list);
   return log;
}

int main(int argc, char** argv) {
   lain::Arguments args = lain::ArgumentBuilder()
      .arg('x').required().option()
      .arg('y').required().option()
      .arg('n').required().option()
      .arg('c', "compound")
      .arg('L', "log").option()
      .parse(lain::str::argv_to_vector(argc, argv));
   
   lain::Settings log;
   std::string log_filename = args.option("log");
   std::set<Point> visited_points;
   int x = std::stoi(args.option('x'));
   int y = std::stoi(args.option('y'));
   unsigned int n = std::stoi(args.option('n'));
   unsigned int total_points = x * y;
   std::random_device rd;
   std::mt19937 mt(rd());
   std::uniform_int_distribution<int> x_dist(1, x);
   std::uniform_int_distribution<int> y_dist(1, y);

   try {
      log = lain::Settings::load_from_file(log_filename);
   } catch (const lain::FileException& e) {
      tfm::format(std::cerr, "Log file '%s' does not exist yet.\n", log_filename);
      log.set_object_array("points", std::vector<lain::Settings>());
   }

   visited_points = read_from_log(log);
   unsigned int target_n = visited_points.size() + n;
   
   if (total_points - visited_points.size() < n) {
      tfm::format(std::cerr, "Not enough points remain in the grid.\n");
      return 1;
   }

   while (visited_points.size() < target_n) {
      Point pt(x_dist(mt), y_dist(mt));
      if (visited_points.find(pt) == visited_points.end()) {
         visited_points.insert(pt);
      }
   }
   
   log = write_to_log(visited_points);
   log.save_to_file(log_filename, true);
   tfm::format(std::cerr, "Log file written to '%s'.\n", log_filename);
}
