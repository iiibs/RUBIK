// Gneneral includes
#if true
#include <vector>
 using std::vector;
#include <string>
 using std::string;
#include <sstream>
 using std::ostringstream;
#include <utility>
 using std::pair;
#include <iostream>
 using std::cout;
 using std::endl;
 using std::cin;
#include <iomanip>
 using std::setw;
 using std::setfill;
#include <functional>
 using std::function;
#include <fstream>
 using std::ifstream;
#include <map>
 using std::map;
#include <set>
 using std::set;
#include <ctime>
 using::time;
#include "nlohmann/json.hpp"
 using json=nlohmann::json;
#endif

#include "config.cpp"
#include "..\FSM\Fsm.hpp"

// Global variables.
json settings;
Fsm fsm("");
struct Puzzle;
Puzzle* puzzle;

// No-operation.
inline void noop() {}

// FSM - Conditions.
#if true
bool is_start(Fsm& fsm) { return true; }
bool is_rightarrow(Fsm& fsm) { return fsm.str[fsm.i_str]=='>'; }
bool is_validchar(Fsm& fsm) { return (fsm.str[fsm.i_str]!=' ')&&(fsm.str[fsm.i_str]!='>'); }
bool is_space(Fsm& fsm) { return fsm.str[fsm.i_str]==' '; }
bool is_nomorechars(Fsm& fsm) { return fsm.i_str>=fsm.str.length(); }
#endif
// FSM - Actions.
#if true
void next_char(Fsm& fsm) { fsm.i_str++; }
void readto_buffer(Fsm& fsm) { fsm.buffer+=fsm.str[fsm.i_str]; fsm.i_str++; }
void delete_buffer(Fsm& fsm) { fsm.buffer=""; fsm.i_str++; }
void no_action(Fsm& fsm) { }
#endif
// FSM - init
#if true
void init_fsm(Fsm& fsm,string str)
{
 fsm.str=str;
 fsm.i_str=0;
 fsm.i_step=0;
 fsm.current_state="Initial";
 // FSM - Transitions.
 fsm.add_transition("Initial",         is_start,        next_char,     "BeforeFaces");
 fsm.add_transition("BeforeFaces",     is_space,        next_char,     "BeforeFaces");
 fsm.add_transition("BeforeFaces",     is_validchar,    no_action,     "StartFaces");
 fsm.add_transition("StartFaces",      is_validchar,    readto_buffer, "InFaces");
 fsm.add_transition("InFaces",         is_validchar,    readto_buffer, "InFaces");
 fsm.add_transition("InFaces",         is_rightarrow,   no_action,     "AfterFaces");
 fsm.add_transition("AfterFaces",      is_rightarrow,   delete_buffer, "BeforeDirection");
 fsm.add_transition("BeforeDirection", is_validchar,    readto_buffer, "InDirection");
 fsm.add_transition("InDirection",     is_validchar,    readto_buffer, "InDirection");
 fsm.add_transition("InDirection",     is_space,        no_action,     "AfterDirection");
 fsm.add_transition("AfterDirection",  is_space,        delete_buffer, "BeforeFaces");
 fsm.add_transition("AfterDirection",  is_space,        next_char,     "BeforeFaces");
}
#endif
 
// Types.
// Face - (can be a color) is used both for identifying a Piece and for identifying a Direction
struct Face
{
 char name;
 size_t id;
 bool operator<(const Face& other) const { return name<other.name; }
};
// Faces - a set and a vector of the Face structure
struct Faces
{
 set<Face> set;
 vector<const Face*> vector;
 void add(Face face)
 {
  face.id=vector.size()+1;
  auto [it_face,inserted]=set.insert(face);
  if(inserted)
  {
   const Face* p_face=&(*it_face);
   vector.push_back(p_face);
  }
 }
 void print_by_name() const
 {
  for(const auto& face:set)
  {
   cout<<"Face name:"<<face.name<<" id= "<<face.id<<endl;
  }
 }
 void print_by_id() const
 {
  cout<<"------------------------------------"<<endl;
  cout<<"puzzle faces"<<endl;
  for(size_t i_face=0;i_face<vector.size();++i_face)
  {
   const Face* face=vector[i_face];
   cout<<"id= "<<vector[i_face]->id<<" name: "<<vector[i_face]->name<<endl;
  }
 }
};
// Piece - in case of the Pocket Cube a Piece is identified by three Face (the colors that can be seen on it)
struct Piece
{
 string str_faces;
 size_t id;
 char get_face_name(size_t i_face){ return 'X'; }
 bool operator<(const Piece& other) const{ return str_faces<other.str_faces; }
};
// Pieces - a set and a vector of the Piece structure
struct Pieces
{
 set<Piece> set;
 vector<const Piece*> vector;
 void add(Piece piece)
 {
  piece.id=vector.size()+1;
  auto [it_piece,inserted]=set.insert(piece);
  if(inserted)
  {
   vector.push_back(const_cast<Piece*>(&(*it_piece)));
  }
 }
 void print_by_name() const
 {
  for(const auto& piece:set)
  {
   cout<<"Piece faces:"<<piece.str_faces<<" id= "<<piece.id<<endl;
  }
 }
 void print_by_id() const
 {
  cout<<"------------------------------------"<<endl;
  cout<<"puzzle pieces"<<endl;
  for(size_t i_piece=0;i_piece<vector.size();++i_piece)
  {
   const Piece* piece=vector[i_piece];
   cout<<"id= "<<vector[i_piece]->id<<" faces: "<<vector[i_piece]->str_faces<<endl;
  }
 }
};
// Location - there are as many Location as many Piece, but the order of Lcations is fixed
struct Location
{
 string str_faces;
 size_t id;
 size_t offset;
 bool operator<(const Location& other) const { return str_faces<other.str_faces; }
};
// Locations - a set and a vector of the Location structure
struct Locations
{
 set<Location> set;
 vector<const Location*> vector;
 void add(Location location)
 {
  location.id=vector.size()+1;
  auto [it_location,inserted]=set.insert(location);
  if(inserted)
  {
   vector.push_back(&(*it_location));
  }
 }
 void print_by_name() const
 {
  for(const auto& location:set)
  {
   cout<<"Location faces:"<<location.str_faces<<" id= "<<location.id<<endl;
  }
 }
 void print_by_id() const
 {
  cout<<"------------------------------------"<<endl;
  cout<<"puzzle locations"<<endl;
  for(size_t i_location=0;i_location<vector.size();++i_location)
  {
   cout<<"id= "<<vector[i_location]->id<<" faces: "<<vector[i_location]->str_faces<<endl;
  }
 }
};
// Position - a Piece in a certain Location facing a certain Direction
struct Position
{
 size_t id;
 size_t location_id;
 size_t direction_id;
};
/*
struct Positions
{
 vector<Position> vector;
 void add(Position position)
 {
  position.id=vector.size()+1;
  vector.push_back(position);
 }
 void print_by_id() const
 {
  for(size_t i=0;i<vector.size();++i)
  {
   const Position& position=vector[i];
   cout<<
    "Position id="<<position.id<<
    " location: "<<position.location.str_faces<<
    " direction: "<<position.direction.name<<
   endl;
  }
 }
};
*/
// Element - a Piece in a Position is an Element of a State, but the order of Elements is fixed
struct Element
{
 size_t id;
 size_t piece_id;
 size_t rotated;
 //size_t position_id;
 //size_t location_id;
 //Position position;
 //Piece piece;
 //bool operator<(const Element& other) const
 //{
 // const auto& a_location=position.location.str_faces;
 // const auto& a_direction = position.direction.name;
 // const auto& b_location = other.position.location.str_faces;
 // const auto& b_direction = other.position.direction.name;
 // return tie(a_location,a_direction)<tie(b_location,b_direction);
// }
};
// State - Elements make up a State, it can be base State, permuted State, source State, target State
struct State
{
 string name;
 string str;
 vector<Element> elements;
 void print_by_id();
 void print_by_name();
};
// Place - used for holding a bool value when processing a Permutation to Cycles
struct Place
{
 bool b_free;
 size_t index;
};
// Move - a Piece is moved by a Permutation from a Location to another Location, and the Direction changes by adding to it modulo 3
struct Move
{
 size_t location_before;
 size_t location_after;
 size_t rotation;
};
// Cycle - the Moves of an element when performing the same Permutation several times until it returns
struct Cycle
{
 vector<Move> moves;
};
// Permutation - has a name and a description how it moves Elements of a State to another State
struct Permutation
{
 string name;
 vector<Place> places;
 vector<Move> moves;
 vector<Cycle> cycles;
};
// Puzzle - all components including Faces, Pieces, Locations, Positions, Elements, States, Moves, Permutations
struct Puzzle
{
 unsigned int id;
 string name;
 Faces faces;
 Pieces pieces;
 size_t n_directions;
 Locations locations;
 State base_state;
 vector<State> permuted_states;
 State source_state;
 vector<Position> positions;
 State target_state;
 State current_state;
 vector<Permutation> permutations;
 void get_puzzle_strings(const vector<string>&);
 void base_state_str_to_faces_pieces_locations(string);
 void setup_positions();
 vector<Element> setup_elements(string);
 void setup_permutations_moves_cycles();
 void get_task_strings(const vector<string>&);
 bool is_solved();
 void apply_permutation(const Permutation&);
 size_t get_direction_of_face(Face);
 size_t get_piece_firstface_id(Piece);
 void setup_target_state();
 void setup_task(string);
 void set_source_state_str_and_target_state_str(const vector<string>&);
 size_t get_rotation(Piece piece,Location location,char);
};

//using t_index=size_t;

//using t_evaluation=vector<size_t>;
//using t_map_name_index=map<string,size_t>;

// Global variables.
unsigned int task_id;
// The base state can be any state, but it is typically the so called "solved state".
// The source state can be any state, but it is typically the so called "scrambled state".
// The target state can be any state, but it is typically the so called "solved state".
//t_state base_state; // every permutation starts from this state
//t_permutations permutations; // the end state of the individual permutations
//t_state source_state; // the scrambled state that must be solved
//t_state target_state; // the target state, which should be the result of the solving process
// When we read in the base state from file, then we convert its [string] [string] notation to integers
//  but also store the <string name, size_t index> pairs for reference
//t_elements_map elements_map;

// Delayed function definitions.
// Some of these functions depend on the full definition of another sucture,
// which is not available at the time of their declaration.
// That's why they are defined here.

 #pragma region Puzzle delayed functions
 void Puzzle::get_puzzle_strings(const vector<string>& lines)
 {
  bool b_processing=false;
  size_t n_rows=0;
  size_t n_cols=0;
  vector<string> base_state_lines;
  if(lines[0]=="Base state")
  {
   for(const std::string &line : lines)
   {
    if(line[0]=='-' && !b_processing)
    {
     b_processing=true;
     n_cols=line.length();
     continue;
    }
    if(line[0]=='-' && b_processing)
    {
     break;
    }
    if(b_processing)
    {
     base_state_lines.push_back(line);
     n_rows++;
    }
   }
  }
  else
  {
   cout<<"Section does not start with 'Base state' line"<<endl;
  }
  cout<<"Status: "<<endl;
  cout<<" n_rows: "<<n_rows<<endl;
  cout<<" n_cols: "<<n_cols<<endl;
  cout<<"Lines: "<<endl;
  for(size_t i_line=0;i_line<base_state_lines.size();++i_line)
  {
   cout<<base_state_lines[i_line]<<endl;
  }
  size_t n_permutation_lines=
   lines.size()
   -1 //the "Start state" row
   +1 //the opening "------" row
   -n_rows //number of rows in a state description
   -1 //the the closing "------" row
   -1 //the "Permutations" row
   -1 //the opening "------" row
   ;
  size_t n_permutations=n_permutation_lines/(
   1 //the "P name" row
   +1 //the opening "------" row
   +n_rows //number of rows in a state description
   +1 //the the closing "------" row
   );
  cout<<"n_permutations: "<<n_permutations<<endl;
  size_t offset_base_state=
   0 //the "Start state" row
   +1 //the opening "------" row
   +1; //the first line of the start state
  string str_base_state;
  for(size_t i_row=offset_base_state;i_row<offset_base_state+n_rows;i_row++)
  {
   str_base_state+=lines[i_row];
  }
  base_state.str=str_base_state;
  if(settings["detailed"])
   cout<<"Base state: "<<str_base_state<<endl;
  for(int i_permutation=0;i_permutation<n_permutations;i_permutation++)
  {
   string str_permuted_state="";
   size_t offset_permuted_state=
    +1 //the "Solved state" row
    +1 //the opening "------" row
    +n_rows //number of rows in a state description
    +1 //the the closing "------" row
    +1 //the "Permutations" row
    +1 //the opening "------" row
    +1 //the permutation name row
    +1 //the permutation opening "------" row
    +i_permutation*(
     +1 //permutation name row
     +1 //the permutation opening "------" row
     +n_rows
     +1 //the permutation opening "------" row
    );
   for(size_t i_row=0;i_row<n_rows;i_row++)
   {
    str_permuted_state+=lines[offset_permuted_state+i_row];
   }
   string str_permutation_name=lines[offset_permuted_state
                                     -1 // --- above the first permuted state line
                                     -1 // permutation name line
                                    ];
   if(settings["detailed"])
   {
    cout<<"Permutation: "<<str_permutation_name<<endl;
    cout<<str_base_state<<" -> "<<str_permuted_state<<endl;
   }
   State permuted_state;
   permuted_state.name=str_permutation_name;
   permuted_state.str=str_permuted_state;
   permuted_states.push_back(permuted_state);
  }
  if(settings["detailed"])
  {
   cout<<"base_state.str: '"<<base_state.str<<"'"<<endl;
  }
  n_directions=base_state.str.find('>')-base_state.str.find_first_not_of(' ');
  cout<<"n_directions="<<n_directions<<endl;
 }
 void Puzzle::base_state_str_to_faces_pieces_locations(string base_state_str)
 {
  if(settings["detailed"])
   cout<<base_state_str<<endl;
  Fsm fsm(base_state_str);
  if(settings["detailed"])
   fsm.detailed=true;
  init_fsm(fsm,base_state_str);
  fsm.str=base_state_str;
  fsm.run();
  vector<Element> elements;
  Piece piece;
  Location location;
  Position position;
  size_t offset;
  for(size_t i_step=0;i_step<fsm.steps.size();i_step++)
  {
   if(settings["detailed"])
    cout<<
     "before action:"<<
     " i_step="<<fsm.steps[i_step].i_step<<
     " i_str="<<fsm.steps[i_step].i_str<<
     " state="<<fsm.steps[i_step].state_from<<
     " char='"<<fsm.str[fsm.steps[i_step].i_str]<<"'"<<
     " after action:"<<
     " state="<<fsm.steps[i_step].state_to<<
     " buffer="<<fsm.steps[i_step].buffer_value<<
    endl;
   string state_reached=fsm.steps[i_step].state_to;
   vector<size_t> directions;
   if(state_reached=="StartFaces")
   {
    offset=fsm.steps[i_step].i_str;
    continue;
   }
   if(state_reached=="AfterFaces")
   {
    string str_faces=fsm.steps[i_step].buffer_value;
    if(settings["detailed"])
     cout<<" Faces: "<<str_faces<<endl;
    piece.str_faces=str_faces;
    location.str_faces=str_faces;
    location.offset=offset;
    for(size_t i_face=0;i_face<piece.str_faces.size();i_face++)
    {
     Face face;
     face.name=piece.str_faces[i_face];
     faces.add(face);
    }
    pieces.add(piece);
    locations.add(location);
    continue;
   }
   if(state_reached=="AfterDirection")
   {
    char direction;
    direction=fsm.steps[i_step].buffer_value[0];
    if(settings["detailed"])
     cout<<" Direction: "<<direction<<endl;
    continue;
   }
  }
 }
 void Puzzle::setup_positions()
 {
  cout<<"------------------------------------"<<endl;
  cout<<"puzzle positions (locations x directions)"<<endl;
  for(size_t i_location=0;i_location<locations.vector.size();++i_location)
  {
   for(size_t i_direction=0;i_direction<n_directions;++i_direction)
   {
    Position position;
    size_t location_id=locations.vector[i_location]->id;
    size_t location_offset=locations.vector[i_location]->offset;
    string location_str=locations.vector[i_location]->str_faces;
    position.id=i_location*n_directions+i_direction+1;
    position.location_id=location_id;
    //position.location.offset=location_offset;
    //position.location.str_faces=location_str;
    position.direction_id=i_direction+1;
    positions.push_back(position);
    cout<<
     "position id="<<position.id<<
     " location id="<<position.location_id<<
     //" offset="<<position.location.offset<<
     //" faces: "<<position.location.str_faces<<
     " direction_id="<<position.direction_id<<
    endl;
   }
  }
  cout<<"------------------------------------"<<endl;
 }
 vector<Element> Puzzle::setup_elements(string str)
 {
  vector<Element> elements;
  cout<<str<<endl;
  cout<<"elements"<<endl;
  for(size_t i_location=0;i_location<locations.vector.size();++i_location)
  {
   Element element;
   element.id=i_location+1;
   size_t location_offset=locations.vector[i_location]->offset;
   string location_faces=locations.vector[i_location]->str_faces;
   Piece piece;
   piece.str_faces=str.substr(location_offset,n_directions);
   piece=*pieces.set.find(piece);
   size_t rotated;
   char facing=str.substr(
    location_offset // 
    +1
    +n_directions,
    1
   )[0];
   rotated=get_rotation(piece,*locations.vector[i_location],facing);
   cout<<
    " location id="<<element.id<<
    " location faces="<<location_faces<<
    " piece id="<<piece.id<<
    " faces:"<<piece.str_faces<<
    " facing:"<<facing<<
    " rotated="<<rotated<<endl;
   element.piece_id=piece.id;
   element.rotated=rotated;
   elements.push_back(element);
   /*
   for(size_t i_position=0;i_position<positions.size();++i_position)
   {
    if(positions[i_position].location_id==piece.id&&
       positions[i_position].direction_id==rotated)
    {
     element.piece_id=piece.id;
     / *
     element.position_id=positions[i_position].id;
     cout<<" position_id= "<<element.position_id<<endl;
     * /
     elements.push_back(element);
     break;
    }
   }
   */
  }
  cout<<"------------------------------------"<<endl;
  return elements;
 }
 void Puzzle::setup_permutations_moves_cycles()
 {
  for(size_t i_permutation=0;i_permutation<permuted_states.size();++i_permutation)
  {
   permuted_states[i_permutation].elements=setup_elements(permuted_states[i_permutation].str);
   Permutation permutation;
   permutation.name=permuted_states[i_permutation].name;
   cout<<permutation.name<<endl;
   permutations.push_back(permutation);
   // Moves
   for(size_t i_element=0;i_element<permuted_states[i_permutation].elements.size();++i_element)
   {
    // from element to piece - location - direction
    Element permuted_element;
    permuted_element=permuted_states[i_permutation].elements[i_element];
    size_t permuted_piece_id;
    permuted_piece_id=permuted_element.piece_id;
    size_t permuted_rotated;
    permuted_rotated=permuted_element.rotated;
    /*
    size_t permuted_location_id;
    permuted_location_id=positions[permuted_position_id-1].location_id;
    size_t permuted_direction;
    permuted_direction=positions[permuted_position_id-1].direction_id;
    */
    Element base_element;
    base_element=base_state.elements[i_element];
    size_t base_piece_id;
    base_piece_id=base_element.piece_id;
    size_t base_rotated;
    base_rotated=base_element.rotated;
    /*
    size_t base_location_id;
    base_location_id=positions[base_position_id-1].location_id;
    size_t base_direction;
    base_direction=positions[base_position_id-1].direction_id;
    */
    if(settings["detailed"])
    {
     cout<<" permuted_element.id="<<permuted_element.id;
     cout<<" permuted_piece_id="<<permuted_piece_id;
     //cout<<" permuted_position_id="<<permuted_position_id;
     //cout<<" permuted_location_id="<<permuted_location_id;
     cout<<" permuted_rotated="<<permuted_rotated;
     cout<<" <-\n";
     cout<<" base_element.id="<<base_element.id;
     cout<<" base_piece_id="<<base_piece_id;
     cout<<" base_rotated="<<base_rotated;
    }
    if
    (
     permuted_piece_id!=permuted_element.id||
     permuted_rotated!=base_rotated
    )
    {
     Move move;
     move.location_before=permuted_piece_id;
     move.location_after=permuted_element.id;
     //move.rotation=permuted_rotated-base_rotated;
     move.rotation=(permuted_rotated-base_rotated+n_directions)%n_directions;
     permutations[i_permutation].moves.push_back(move);
     cout<<" piece "<<permuted_piece_id;
     cout<<" moved to "<<permuted_element.id<<",";
     cout<<" while rotated "<<move.rotation;
     cout<<endl;
    }
   }
   // Cycles
   map<size_t,Move> move_map; // from -> to
   for (const Move& move : permutations[i_permutation].moves)
   {
    move_map[move.location_before]=move;
   }
   set<size_t> visited;
   for (const auto& [start_id, move] : move_map)
   {
    if (visited.count(start_id)) continue;
    Cycle cycle;
    size_t current = start_id;
    while (!visited.count(current))
    {
     visited.insert(current);
     const Move& m = move_map[current];
     cycle.moves.push_back(m);
     current = m.location_after;
    }
    if (cycle.moves.size() > 1)
    {
     permutations[i_permutation].cycles.push_back(cycle);
     cout << " Cycle: ";
     for (const Move& m : cycle.moves)
      cout << m.location_before << " -> ";
     cout << cycle.moves.back().location_after << endl;
    }
   }
  }
 }
 void Puzzle::get_task_strings(const vector<string>& lines)
 {
  bool b_processing=false;
  size_t n_rows=0;
  size_t n_cols=0;
  vector<string> source_state_lines;
  if(lines[0]=="Source state")
  {
   for(const std::string &line : lines)
   {
    if(line[0]=='-' && !b_processing)
    {
     b_processing=true;
     n_cols=line.length();
     continue;
    }
    if(line[0]=='-' && b_processing)
    {
     break;
    }
    if(b_processing)
    {
     source_state_lines.push_back(line);
     n_rows++;
    }
   }
  }
  else
  {
   cout<<"Section does not start with 'Source state' line"<<endl;
  }
  if(settings["detailed"])
  {
   cout<<"Status: "<<endl;
   cout<<" n_rows: "<<n_rows<<endl;
   cout<<" n_cols: "<<n_cols<<endl;
   cout<<"Lines: "<<endl;
   for(size_t i_line=0;i_line<source_state_lines.size();++i_line)
   {
    cout<<source_state_lines[i_line]<<endl;
   }
  }
  size_t offset_source_state=
   0 //the "Start state" row
   +1 //the opening "------" row
   +1; //the first line of the start state
  string str_source_state;
  for(size_t i_row=offset_source_state;i_row<offset_source_state+n_rows;i_row++)
  {
   str_source_state+=lines[i_row];
  }
  source_state.str=str_source_state;
  //if(settings["detailed"])
   cout<<"Source state: "<<source_state.str<<endl;
  string str_target_state="";
  size_t offset_target_state=
   +1 //the "Source state" row
   +1 //the opening "------" row
   +n_rows //number of rows in a state description
   +1 //the the closing "------" row
   +1 //the "Target state" row
   +1 //the opening "------" row
   ;
   for(size_t i_row=0;i_row<n_rows;i_row++)
   {
    str_target_state+=lines[offset_target_state+i_row];
   }
   string str_target_name=lines[offset_target_state
                                     -1 // --- above the first permuted state line
                                     -1 // permutation name line
                                    ];
  target_state.name=str_target_name;
  target_state.str=str_target_state;
  //if(settings["detailed"])
   cout<<"Target state: "<<target_state.str<<endl;
 }
 bool Puzzle::is_solved()
 {
  for(size_t i_element=0;i_element<current_state.elements.size();++i_element)
  {
   const Element& curr=current_state.elements[i_element];
   const Element& target=target_state.elements[i_element];
   if(curr.piece_id!=target.piece_id||curr.rotated!=target.rotated)
    return false;
  }
  return true;
 }
 void Puzzle::apply_permutation(const Permutation& permutation)
 {
  State new_state = current_state;
  for (const auto& move : permutation.moves)
  {
   const Element& from_element = current_state.elements[move.location_before-1];
   Element moved = from_element;
   moved.rotated = (from_element.rotated + move.rotation) % n_directions;
   new_state.elements[move.location_after-1] = moved;
  }
  current_state = new_state;
 }
 /*
 void Puzzle::apply_permutation_debug(Permutation permutation)
 {
  State new_state=current_state;
  new_state.print_by_id();
  for(size_t i_element=0;i_element<current_state.elements.size();++i_element)
  {
   cout<<"a current State aktualisan vizsgalt Element-je: ";
   size_t element_id=current_state.elements[i_element].id;
   cout<<element_id<<endl;
   cout<<"ez az Element jelenleg a kovetkezo Piece-t tartalmazza: ";
   size_t piece_id=current_state.elements[i_element].piece_id;
   cout<<piece_id<<endl;
   cout<<"melynek megjelenese: ";
   string str_faces=pieces.vector[current_state.elements[i_element].piece_id-1]->str_faces;
   cout<<str_faces<<endl;
   cout<<"es ez a Piece ennyivel van elforgatva: ";
   size_t rotated=current_state.elements[i_element].rotated;
   cout<<rotated<<endl;
   for(size_t i_move=0;i_move<permutation.moves.size();++i_move)
   {
    cout<<"ez a Permutacio aktualisan vizsgalt Move-ja:"<<endl;
    cout<<" Location before: "<<permutation.moves[i_move].location_before<<endl;
    cout<<" Location after: "<<permutation.moves[i_move].location_after<<endl;
    cout<<" Rotation: "<<permutation.moves[i_move].rotation<<endl;
    cout<<"---"<<endl;
    cout<<"element id: "<<current_state.elements[i_element].id<<endl;
    cout<<"permutation.moves[i_move].location_before: "<<permutation.moves[i_move].location_before<<endl;
    cout<<"---"<<endl;
    if(current_state.elements[i_element].id==permutation.moves[i_move].location_before-1)
    {
     new_state.elements[i_element]=current_state.elements[permutation.moves[i_move].location_before-1];
    }
   }
  }
  current_state=new_state;
 }
 */
 size_t Puzzle::get_direction_of_face(Face face)
 {
  size_t face_id=-1;
  return face_id;
 }
 size_t Puzzle::get_piece_firstface_id(Piece piece)
 {
  size_t firstface_id=-1;
  return firstface_id;
 }
 void Puzzle::setup_target_state()
 {
  /*
  if(settings["detailed"])
   cout<<base_state.str<<endl;
  if(settings["detailed"])
   fsm.detailed=true;
  fsm.str=base_state.str;
  fsm.run();
  Piece piece;
  Location location;
  Element element;
  Position position;
  size_t offset;
  // Determine the number of: Face, Piece, Location, Direction, Position, Element of the Puzzle
  for(size_t i_step=0;i_step<fsm.steps.size();i_step++)
  {
   if(settings["detailed"])
    cout<<
     "before action:"<<
     " i_step="<<fsm.steps[i_step].i_step<<
     " i_str="<<fsm.steps[i_step].i_str<<
     " state="<<fsm.steps[i_step].state_from<<
     " char='"<<fsm.str[fsm.steps[i_step].i_str]<<"'"<<
     " after action:"<<
     " state="<<fsm.steps[i_step].state_to<<
     " buffer="<<fsm.steps[i_step].buffer_value<<
    endl;
   string state_reached=fsm.steps[i_step].state_to;
   vector<size_t> directions;
   if(state_reached=="StartFaces")
   {
    offset=fsm.steps[i_step].i_str;
    continue;
   }
   if(state_reached=="AfterFaces")
   {
    string str_faces=fsm.steps[i_step].buffer_value;
    if(settings["detailed"])
     cout<<" Faces: "<<str_faces<<endl;
    piece.str_faces=str_faces;
    location.str_faces=str_faces;
    for(size_t i_face=0;i_face<piece.str_faces.size();i_face++)
    {
     faces.add(piece.str_faces[i_face]);
    }
    pieces.add(piece.str_faces);
    locations.items.push_back(location);
    continue;
   }
   if(state_reached=="AfterDirection")
   {
    string str_direction;
    str_direction=fsm.steps[i_step].buffer_value[0];
    if(settings["detailed"])
     cout<<" Direction: "<<str_direction<<endl;
    element.piece.str_faces=piece.str_faces;
    element.position.location.str_faces=element.piece.str_faces;
    element.position.direction.name=str_direction[0];
    element.position.offset=offset;
    base_state.elements.add(element);
    continue;
   }
  }
  //locations.setup();
  cout<<"puzzle base state"<<endl;
  size_t id=0;
  for(const Element& element:base_state.elements.items)
  {
   const_cast<Element&>(element).id=id++;  // Safe if name used for sorting
   cout<<" element id: "<<element.id<<endl;
   cout<<"  element piece faces: "<<element.piece.str_faces<<endl;
   string str_faces=element.piece.str_faces;
   //cout<<str_faces<<endl;
   //auto it=pieces.items.begin();
   Piece piece;
   piece.str_faces=element.piece.str_faces;
   auto it_piece=pieces.items.find(piece);
   //cout<<it_piece->str_faces<<endl;
   const_cast<Element&>(element).piece.id=it_piece->id;
   cout<<"   element piece id: "<<element.piece.id<<endl;
   Position position;
   position.direction=element.position.direction;
   auto it_face=faces.set.find(position.direction);
   cout<<"  element position direction name: "<<element.position.direction.name<<endl;
   const_cast<Element&>(element).position.direction.id=it_face->id;
   cout<<"   element position direction id: "<<element.position.direction.id<<endl;
   //Location location;
   //location.str_faces=element.position.location.str_faces;
   //auto it_location=locations.items.find(location);
   //cout<<"  element position location faces: "<<element.position.location.str_faces<<endl;
   //const_cast<Element&>(element).position.location.id=it_location->id;
   //cout<<"   element position location id: "<<element.position.location.id<<endl;
   cout<<endl;
  }
  */
 }
 void Puzzle::setup_task(string state_str)
 {
  /*
  process_state(base_state.str);
  */
 }
 void Puzzle::set_source_state_str_and_target_state_str(const vector<string>& lines)
 {
  bool b_processing;
  size_t n_rows;
  size_t n_cols;
  b_processing=false;
  n_rows=0;
  n_cols=0;
  size_t line_offset=0;
  if(lines[line_offset]=="Source state")
  {
   do
   {
    string line=lines[line_offset];
    if(line[0]=='-'&&!b_processing)
    {
     b_processing=true;
     n_cols=line.length();
     line_offset++;
     continue;
    }
    if(line[0]=='-'&&b_processing)
    {
     line_offset++;
     break;
    }
    if(b_processing)
    {
     source_state.str+=line;
     n_rows++;
    }
    line_offset++;
   }
   while(true);
  }
  else
  {
   cout<<"Section does not start with 'Source state' line"<<endl;
  }
  cout<<"Source state: "<<endl;
  cout<<" n_rows: "<<n_rows<<endl;
  cout<<" n_cols: "<<n_cols<<endl;
  cout<<source_state.str<<endl;
  b_processing=false;
  n_rows=0;
  n_cols=0;
  if(lines[line_offset]=="Target state")
  {
   do
   {
    string line=lines[line_offset];
    if(line[0]=='-'&&!b_processing)
    {
     b_processing=true;
     n_cols=line.length();
     line_offset++;
     continue;
    }
    if(line[0]=='-'&&b_processing)
    {
     line_offset++;
     break;
    }
    if(b_processing)
    {
     target_state.str+=line;
     n_rows++;
    }
    line_offset++;
   }
   while(true);
  }
  else
  {
   cout<<"Section does not start with 'Target state' line"<<endl;
  }
  cout<<"Target state: "<<endl;
  cout<<" n_rows: "<<n_rows<<endl;
  cout<<" n_cols: "<<n_cols<<endl;
  cout<<target_state.str<<endl;
 }
 size_t Puzzle::get_rotation(Piece piece,Location location,char facing)
 {
  char piece_first_face=piece.str_faces[0];
  char location_face;
  size_t rotation;
  for(rotation=0;rotation<n_directions;rotation++)
  {
   location_face=location.str_faces[rotation];
   if(facing==location_face)
    break;
  }
  return rotation;
 }
 #pragma endregion

 #pragma region State delayed functions
 void State::print_by_id()
 {
  cout<<"E "<<"P "<<"R "<<endl;
  for(size_t i_element=0;i_element<elements.size();++i_element)
  {
   cout<<i_element+1<<" "<<elements[i_element].piece_id<<" "<<elements[i_element].rotated<<endl;
  }
 }
 void State::print_by_name()
 {
  for(size_t i_element=0;i_element<elements.size();++i_element)
  {
   puzzle->pieces.print_by_name();
   cout<<endl;
   Piece piece;
   //piece= elements[i_element].piece_id;
   //cout<<elements[i_element].id<<" "<<elements[i_element].piece_id<<" "<<elements[i_element].position_id<<endl;
  }
 }
 #pragma endregion

// Main menu - Basic setup.
void basic_setup()
{
 cout<<"Puzzle name: '"<<puzzle->name<<"'"<<endl;
};

// Main menu - Advanced setup.
/*
static t_permutation extract_permutation(string str_base,string str_after)
{
 // [A][B][C] -> [B][A][C]
 // "A" -> index: 0, startposition_before: 1, startposition_after: 4
 // "B" -> index: 1, startposition_before: 4, startposition_after: 1
 // "C" -> index: 2, startposition_before: 7, startposition_after: 7
 size_t pos;
 pos=0;
 while((pos=str_base.find('[',pos))!=string::npos)
 {
  size_t end=str_base.find(']',pos);
  if(end!=string::npos)
  {
   string element=str_base.substr(pos+1,end-pos-1);
   size_t baseposition=pos+1;
   elements_baseposition[element]=baseposition;
   if(settings["detailed"])
    cout<<'"'<<element<<'"'<<" "<<baseposition<<endl;
   pos=end+1;
  }
 }
 map<string,size_t> element_afterpositions;
 pos=0;
 while((pos=str_after.find('[',pos))!=string::npos)
 {
  size_t end=str_after.find(']',pos);
  if(end!=string::npos)
  {
   string element=str_after.substr(pos+1,end-pos-1);
   size_t afterposition=pos+1;
   element_afterpositions[element]=afterposition;
   if(settings["detailed"])
    cout<<'"'<<element<<'"'<<" "<<afterposition<<endl;
   pos=end+1;
  }
 }
 // Now create a vector of elements in order of the base state
 vector<string> sorted_elements;
 for (const auto& [element,startpos]:elements_baseposition)
 {
  sorted_elements.push_back(element);
 }
 // Create the result vector: result[i] = j means element at index i moved to index j
 vector<size_t> result(sorted_elements.size());
 for(size_t i=0;i<sorted_elements.size();++i)
 {
  string element=sorted_elements[i];
  size_t afterpos=element_afterpositions[element];
  // Find where this end position matches in the original map
  int j=0;
  for(const auto& [other_element,pos]:elements_baseposition)
  {
   if(pos==afterpos)
   {
    result[i]=j;
    break;
   }
   j++;
  }
 }
 // Print the result vector
 cout<<"Permutation: ";
 for(size_t idx:result)
 {
  cout<<idx<<" ";
 }
 cout<<endl;
 t_permutation permutation;
 return permutation;
}
*/

/*
t_permutation get_permutation(string str_permuted_state)
{
 t_permutation permutation;
 cout<<str_permuted_state<<endl;
 Piece piece;
 Face direction;
 Location location;
 t_move move;
 cout<<endl;
 fsm.str=str_permuted_state;
 fsm.detailed=true;
 fsm.run();
 for(size_t i_step=0;i_step<fsm.steps.size();i_step++)
 {
  if(settings["detailed"])
   cout<<
   "before action:"<<
   " i_step="<<fsm.steps[i_step].i_step<<
   " i_str="<<fsm.steps[i_step].i_str<<
   " state="<<fsm.steps[i_step].state_from<<
   " char='"<<fsm.str[fsm.steps[i_step].i_str]<<"'"<<
   " after action:"<<
   " state="<<fsm.steps[i_step].state_to<<
   " buffer="<<fsm.steps[i_step].buffer_value<<
   endl;
  string state_reached=fsm.steps[i_step].state_to;
  vector<size_t> directions;
  if(state_reached=="AfterDirection")
  {
   string str_direction;
   str_direction=fsm.steps[i_step].buffer_value[0];
   if(settings["detailed"])
    cout<<" Direction: "<<str_direction<<endl;
   direction.name=str_direction[0];
   continue;
  }
  if(state_reached=="AfterFaces")
  {
   string str_faces=fsm.steps[i_step].buffer_value;
   if(settings["detailed"])
    cout<<" Faces: "<<str_faces<<endl;
   piece.str_faces=str_faces;
   location.str_faces=str_faces;
   for(size_t i_face=0;i_face<piece.str_faces.size();i_face++)
   {
    puzzle.faces.add(piece.str_faces[i_face]);
   }
   puzzle.pieces.add(piece.str_faces);
   puzzle.locations.items.push_back(location);
   continue;
  }
 }
 return permutation;
}
*/

/*
t_state get_state(string str_state)
{
 t_state state;
 if(settings["detailed"])
  cout<<str_state<<endl;
 size_t pos=0;
 size_t index=0;
 while((pos=str_state.find('[',pos))!=string::npos)
 {
  size_t end=str_state.find(']',pos);
  if(end!=string::npos)
  {
   string element=str_state.substr(pos+1,end-pos-1);
   size_t position=pos+1;
   if(settings["detailed"])
   {
    cout<<'"'<<element<<'"'<<" "<<elements_map[element].index<<" "<<index<<endl;
   }
   state.push_back(elements_map[element].index);
   index++;
   pos=end+1;
  }
 }
 return state;
}
*/

/*
t_state extract_state(const vector<string> lines,string str_label)
{
 t_state state;
 bool b_processing=false;
 size_t n_rows=0;
 size_t n_cols=0;
 vector<string> state_lines;
 for(size_t i_line=0;i_line<lines.size();i_line++)
 {
  if(lines[i_line]==str_label)
  {
   i_line++;
   if(lines[i_line][0]=='-')
   {
    b_processing=true;
    n_cols=lines[i_line].length();
    continue;
   }
   else
    break;
  }
  if(lines[i_line][0]=='-' && b_processing)
  {
   break;
  }
  if(b_processing)
  {
   state_lines.push_back(lines[i_line]);
   n_rows++;
  }
 }
 string str_state_line="";
 for(size_t i_row=0;i_row<n_rows;i_row++)
 {
  str_state_line+=state_lines[i_row];
 }
 cout<<str_label<<endl;
 cout<<str_state_line<<endl;
 state=get_state(str_state_line);
 return state;
}
*/

static vector<string> read_textfile_lines(string str_textfile_path)
{
 ifstream infile;
 string line;
 infile.open(str_textfile_path);
 vector<string> lines;
 if(!infile)
 {
  cout<<"Error: Could not open file: "<<str_textfile_path<<endl;
  return lines;
 }
 while(getline(infile,line))
 {
  lines.push_back(line);
 }
 infile.close();
 return lines;
}
vector<string> read_puzzle_lines()
{
 string str_textfile_path;
 ostringstream oss;
 oss.str("");
 oss.clear();
 oss<<"D:/BAREK/Rubik/puzzle_"<<setw(3)<<setfill('0')<<puzzle->id<<".txt";
 str_textfile_path=oss.str();
 vector<string> lines;
 lines=read_textfile_lines(str_textfile_path);
 return lines;
}
vector<string> read_task_lines()
{
 string str_textfile_path;
 ostringstream oss;
 oss.str("");
 oss.clear();
 oss<<"D:/BAREK/Rubik/puzzle_"<<setw(3)<<setfill('0')<<puzzle->id<<"_task_"<<setw(3)<<setfill('0')<<task_id<<".txt";
 str_textfile_path=oss.str();
 vector<string> lines;
 lines=read_textfile_lines(str_textfile_path);
 return lines;
}

void advanced_setup()
{
 basic_setup();
 vector<string> lines;
 lines=read_puzzle_lines();
 puzzle->get_puzzle_strings(lines);
 //if(settings["detailed"])
  cout<<"Puzzle base state string:"<<endl<<puzzle->base_state.str<<endl;
 puzzle->base_state_str_to_faces_pieces_locations(puzzle->base_state.str);
 //if(settings["detailed"])
 puzzle->faces.print_by_id();
 puzzle->pieces.print_by_id();
 puzzle->locations.print_by_id();
 puzzle->setup_positions();
 puzzle->base_state.elements=puzzle->setup_elements(puzzle->base_state.str);
 puzzle->setup_permutations_moves_cycles();
 lines=read_task_lines();
 puzzle->get_task_strings(lines);
 puzzle->source_state.elements=puzzle->setup_elements(puzzle->source_state.str);
 puzzle->target_state.elements=puzzle->setup_elements(puzzle->target_state.str);
};

// Main menu - Find cycles.

/*
t_permutation inverse(const t_permutation permutation)
{
 t_permutation inverse_permutation;
 for(const auto &p:permutation)
 {
  inverse_permutation.push_back({p.second,p.first});
 }
 return inverse_permutation;
}
*/

/*
bool is_concurrent(const t_permutation& permutation_1,const t_permutation& permutation_2)
{
 bool concurrent=false;
 if(permutation_1.size()!=permutation_2.size())
  exit(1);
 size_t n_locations=permutation_1.size();
 bool identical=true;
 for(int i_location=0;i_location<n_locations;i_location++)
 {
  if((permutation_1[i_location].first!=permutation_2[i_location].first)or
     (permutation_1[i_location].second!=permutation_2[i_location].second))
  identical=false;
 }
 if(not identical)
 {
  for(int i_location=0;i_location<n_locations;i_location++)
  {
   if((permutation_1[i_location].first!=permutation_1[i_location].second)&&
      (permutation_2[i_location].first!=permutation_2[i_location].second))
    return true;
  }
 }
 else
 {
  return false;
 }
 return concurrent;
}
*/

/*
t_evaluation evaluate_permutation(const t_permutation& permutation)
{
 size_t n_locations=permutation.size();
 t_evaluation cycle_counts(n_locations+1,0);
 vector<bool>visited(n_locations,false);
 for(int i_location=0;i_location<n_locations;i_location++)
 {
  if(not visited[i_location])
  {
   size_t current=i_location;
   int n_steps=0;
   while(not visited[current])
   {
    visited[current]=true;
    if(not (permutation[current].first==permutation[current].second))
    {
     current=permutation[current].second;
     n_steps++;
    }
   };
   cycle_counts[n_steps]++;
  }
 }
 return cycle_counts;
}
*/

/*
bool does_swap_just_two(const t_permutation& permutation)
{
 t_evaluation evaluation;
 evaluation=evaluate_permutation(permutation);
 bool swap_ok=false;
 bool others_ok=true;
 if(evaluation[2]>0)
  swap_ok=true;
 else
  return false;
 for(size_t i_evaluation=0;i_evaluation<evaluation.size();i_evaluation++)
 {
  if(evaluation[i_evaluation]>0)
  {
   others_ok=false;
   break;
  }
 }
 return swap_ok and others_ok;
}
*/

/*
t_permutation united_permutations(t_permutations permutations)
{
 t_permutation united_permutation;
 t_state state=solved_state;
 for(size_t i_permutation=0;i_permutation<permutations.size();i_permutation++)
 {
  t_permutation permutation=permutations[i_permutation];
  state=apply_permutation(state,permutation);
 }
 for(size_t i_location=0;i_location<state.size();i_location++)
 {
  size_t new_location;
  for(size_t j_location=0;j_location<state.size();j_location++)
  {
   if(state[j_location]==solved_state[i_location])
   {
    new_location=j_location;
    break;
   }
  }
  united_permutation.push_back({solved_state[i_location],new_location});
 }
 return united_permutation;
}
*/

/*
t_permutation composed_permutation(const t_permutations& permutations)
{
 t_state state=solved_state;
 size_t n_permutations=permutations.size();
 for(int i_permutation=0;i_permutation<n_permutations;i_permutation++)
 {
  state=apply_permutation(state,permutations[i_permutation]);
 }
 t_permutation composed_permutation;
 //size_t n_pairs=permutation.size();
 return composed_permutation;
}
*/

/*
int get_period(const t_permutation& permutation)
{
 t_state applied_state=base_state;
 int n_applications=0;
 do
 {
  applied_state=apply_permutation(applied_state,permutation);
  n_applications++;
 } while(applied_state!=base_state);
 return n_applications;
}
*/

/*
t_state random_scramble(t_state current_state,int n_steps)
{
 for(int i_step=0;i_step<n_steps;i_step++)
 {
  cout<<"Step "<<i_step<<endl;
  int rand_index=rand()%permutations.size();
  current_state=apply_permutation(current_state,permutations[rand_index]);
  i_step++;
 }
 return current_state;
}
*/

void random_solve()
{
 advanced_setup();
 puzzle->current_state=puzzle->source_state;
 //srand(37); // R+ <- ez a megoldas
 srand(time(nullptr));
 int i_step=0;
 while(puzzle->is_solved()!=true)
 {
  cout<<"Step "<<i_step<<endl;
  int rand_index=rand()%puzzle->permutations.size();
  Permutation permutation=puzzle->permutations[rand_index];
  cout<<permutation.name<<endl;
  puzzle->apply_permutation(permutation);
  puzzle->current_state.print_by_id();
  i_step++;
 }
}

/*
void find_cycles()
{
 basic_info();
 advanced_info();
 puzzle.current_state=puzzle.source_state;
 //puzzle.current_state.print_as_faces();
 //current_state=apply_permutation(current_state,permutations[0]);
 puzzle.current_state.print();
 for(size_t i_permutation=0;i_permutation<permutations.size();i_permutation++)
 {
  cout<<"Period of permutation "<<i_permutation<<": "<<get_period(permutations[i_permutation])<<endl;
 }
 srand((unsigned int)time(nullptr));
 //puzzle.current_state=random_scramble(puzzle.current_state,0);
 cout<<"Scrambled:"<<endl;
 //print_state(puzzle.current_state);
 cout<<"Target:"<<endl;
 print_state(target_state);
 cout<<"Solution steps:"<<endl;
 //random_solve(puzzle.current_state);
};
*/

// Main menu.
void main_menu()
{
 map<int,string> menu_labels=
 {
  {1,"Basic Setup"},
  {2,"Advanced Setup"},
  {3,"Random Solve"},
 };
 map<int, std::function<void()>> menu_actions=
 {
  {1,basic_setup},
  {2,advanced_setup},
  {3,random_solve},
 };
 while(true)
 {
  std::cout << "\nMain Menu:\n";
  std::cout << "----------\n";
  for(const auto& [key, label]:menu_labels)
  {
   cout<<key<<". "<<label<<'\n';
  }
  cout<<"Enter your choice, or 0 to return: ";
  int choice;
  cin>>choice;
  cout<<'\n';
  if(choice==0) break;  // Exit menu
  if(menu_actions.find(choice)!=menu_actions.end())
  {
   menu_actions[choice]();  // Call the function
  }
  else
  {
   cout<<"Invalid choice. Try again.\n";
  }
 }
}

// Main.
int main()
{
 puzzle=new Puzzle;
 settings=load_settings();
 puzzle->id=settings["puzzle_id"];
 puzzle->name=settings["puzzles"][puzzle->id]["name"];
 main_menu();

 return 0;

 /*
 const t_permutations permutations=
 {
  {{0,1},{1,0},{2,2}},
  {{0,0},{1,2},{2,1}},
 };
 t_state state;

 //state={0,1,2,};
 state={2,1,0,};
 
 cout<<""<<endl;
 cout<<"Looking for a series of permutations that will transfer from:"<<endl;
 print_state(state);
 cout<<"to:"<<endl;
 print_state(solved_state);
 cout<<"where the basic permutations are:"<<endl;
 size_t n_permutations=permutations.size();
 for(int i_permutation=0;i_permutation<n_permutations;i_permutation++)
 {
  cout<<i_permutation<<". permutation:"<<endl;
  print_permutation(permutations[i_permutation]);
 }

 cout<<"Strict lower rectangular of the concurrency matrix of the "<<n_permutations<<"."<<endl;
 for(int i_permutation=0;i_permutation<n_permutations;i_permutation++)
 {
  for(int j_permutation=0;j_permutation<n_permutations;j_permutation++)
  {
   if(i_permutation>j_permutation)
   {
    if(is_concurrent({permutations[i_permutation]},permutations[j_permutation]))
     cout<<"Permutation "<<i_permutation<<" and permutation "<<j_permutation<<" are concurrent."<<endl;
    else
    cout<<"Permutation "<<i_permutation<<" and permutation "<<j_permutation<<" independent."<<endl;
   }
  }
 }
 
 cout<<"Evaluating cycles in permutations."<<endl;
 for(size_t i_permutation=0;i_permutation<permutations.size();i_permutation++)
 {
  t_permutation permutation=permutations[i_permutation];
  print_permutation(permutation);
  t_evaluation evaluation=evaluate_permutation(permutation);
  for(size_t i_evaluation=0;i_evaluation<evaluation.size();i_evaluation++)
  {
   cout<<"Permutation "<<i_permutation<<" has "<<evaluation[i_evaluation]<<" "<<i_evaluation<<"-cycles."<<endl;
  }
  if(does_swap_just_two(permutation))
   cout<<"Just swaps two elements, can be used for simple solving!"<<endl;
   //if(does_rotate_just_three(permutation))
   // cout<<"Just swaps two elements, can be used for simple solving!"<<endl;
 }

 cout<<"Generating series of permutations"<<endl;
 t_permutations series;
 series.push_back(permutations[0]);
 print_permutation(series[0]);
 t_permutation seria;
 seria=united_permutations(series);
 print_permutation(seria);
 for(size_t i_permutation=0;i_permutation<permutations.size();i_permutation++)
 {
  t_permutation permutation=permutations[i_permutation];
  if(is_concurrent(seria,permutation))
  {
   series.push_back(inverse(permutation));
   series.push_back(inverse(permutations[0]));
   series.push_back(permutation);
   seria=united_permutations(series);
   cout<<"United permutation"<<endl;
   print_permutation(seria);
   break;
  }
 }
 exit(1);
 */

 /*
 int n_depth=0;
 state=solved_state;
 while(true)
 {
  bool cycle2=false;
  for(const auto &permutation:permutations)
  {
   state=apply_permutation(state,permutation);
   print_state(state);
   if(is_cycle2(state))
   {
    cycle2=true;
    break;
   }
  }
  if(cycle2)
  {
   cout<<"Cycle2 exists at depth "<<n_depth<<"."<<endl;
   break;
  }
  else
  {
   cout<<"No cycle2, depth "<<n_depth<<" is not enough, going deeper."<<endl;
  }
  n_depth++;
 }

 cout<<"The total number of permutations is: "<<n_permutations<<endl;
 for(int n_permutations_in_subset=0;n_permutations_in_subset<=n_permutations;n_permutations_in_subset++)
 {
  cout<<"Tests with "<<n_permutations_in_subset<<" permutations"<<endl;
  t_permutations permutations_subset;
  //permutations_subset=generate_
 }

 cout<<"Check if it is solved state."<<endl;
 if(is_solved(state))
 {
  cout<<"Solved by no permutation."<<endl;
  return 0;
 }
 

 cout<<"Check if one of the permutations simply solves it."<<endl;
 for(const auto &permutation:permutations)
 {
  t_state temp_state=state;
  int max_steps=period(state,permutation)-1;
  for(int i_step=0;i_step<max_steps;i_step++)
  {
   temp_state=apply_permutation(temp_state,permutation);
   print_state(temp_state);
   if(is_solved(temp_state))
   {
    cout<<"Solved by applying a single permutation "<<i_step+1<<" times."<<endl;
   }
  }
 }
 cout<<"Not a state that could be solved by just performing a single permutation repeatedly."<<endl;
 */

 /*
 for(int i=0;i<3;i++)
 {
  state=apply_permutation(state,permutations[0]);
  print_state(state);
  state=apply_permutation(state,inverse(permutations[1]));
  print_state(state);
  state=apply_permutation(state,inverse(permutations[0]));
  print_state(state);
  state=apply_permutation(state,permutations[1]);
  print_state(state);
  cout<<endl;
 }
 */
 return 0;
}
