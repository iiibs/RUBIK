#pragma once

#include <fstream>
 using std::ifstream;
 using std::ofstream;
#include "nlohmann/json.hpp"
 using json=nlohmann::json;

inline void save_settings(json &settings)
{
 ofstream file("settings.json");
 file<<settings.dump(1);  // Write formatted JSON
 file.close();
}

inline json create_settings()
{
 json settings;
 settings=
 {
  {"detailed",false},
  {"puzzle_id",3},
  {"task_id",0},
  {"action","info"},
  {"puzzles",json::array(
   {
    {
     {"id",0},
     {"name","3 elements with 2 cycle-of-2 permutations"},
    },
    {
     {"id",1},
     {"name","4 elements with 2 cycle-of-3 permutations"},
    },
    {
     {"id",2},
     {"name","5 elements with 2 x cycle-of-3 permutations"},
    },
    {
     {"id",3},
     {"name","Pocket Cube 2 x 2 x 2"},
    },
   })
  }
 };
 save_settings(settings);
 return settings;
}

inline json load_settings()
{
 json settings;
 ifstream file("settings.json");
 if (file)
 {
  file>>settings;  // Read and parse JSON
 }
 settings=create_settings();
 return settings;
}
