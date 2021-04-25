#include "NumberWithUnits.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>


using namespace std;
using namespace ariel;
const double percision = .00001;

//Initalizeing the static object
std::unordered_map<std::string,std::unordered_map<std::string,double>>  NumberWithUnits::ConvertionMap;

NumberWithUnits::NumberWithUnits(double val, const string &unit){
    if(ConvertionMap.find(unit)==ConvertionMap.end()){
      throw invalid_argument("Measuremeant unit Doesn't exist");
    }
    Value=val;
    Unit=unit;  
}
//Insert value from text and update according to existing values
void NumberWithUnits::InsertUpdate(const string &val1, const string &val2, double mult){
  ConvertionMap[val1][val2]=mult;
//Iterate over the map of the val1 value
  for(auto const& Key:ConvertionMap[val1]){
    //Continue in case its already exist;
    if(ConvertionMap[Key.first].find(val2)!=ConvertionMap[Key.first].end()){continue;}
    //Alter the value accordingly, and insert both itself and the inverse of itself.
    double temp = mult/Key.second;
    ConvertionMap[Key.first][val2] = temp;
    ConvertionMap[val2][Key.first] = 1/temp;
      }  
    
}

void NumberWithUnits::read_units(ifstream &file){   
  string Line;
  string UnitA;
  string UnitB; 
  double tempValue = 0;
  while (getline(file, Line)){
    //get left Unit without the 1 and erase it's spaces
    UnitA = Line.substr(1,Line.find('=')-1);
    UnitA.erase(remove(UnitA.begin(), UnitA.end(), ' '),UnitA.end()); 
    //get the right unit and seperate the value and the unit
    UnitB = Line.substr(Line.find('=')+1);  
    UnitB.erase(remove(UnitB.begin(), UnitB.end(), ' '),UnitB.end());
    istringstream Bstream(UnitB);
    Bstream >> tempValue;
    Bstream >> UnitB;
    //send them to the hash map using a function
    InsertUpdate(UnitA,UnitB,tempValue);
    InsertUpdate(UnitB,UnitA,1/tempValue);
  }
}
//Convert using the hashmap values.
double NumberWithUnits::UnitConvert(const string &from,const string &to, double value){
  if(from==to){
    return value;}
  if(NumberWithUnits::ConvertionMap[from].find(to)==NumberWithUnits::ConvertionMap[from].end()){    
    throw invalid_argument("Units are not convertable.");
  }
  return ConvertionMap[from][to]*value;
}
const NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& b){
  Value -= UnitConvert(b.Unit,Unit,b.Value);
  return *this;  
}
const NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& b){
  Value+=UnitConvert(b.Unit,Unit,b.Value);
  return *this;
}

NumberWithUnits NumberWithUnits::operator++(int flag){
  Value++;
  return NumberWithUnits{Value-1,Unit};}

NumberWithUnits& NumberWithUnits::operator++(){  
  Value++;
  return *this;}

NumberWithUnits NumberWithUnits::operator--(int flag) {
  Value--;
  return NumberWithUnits{Value+1,Unit};}

NumberWithUnits& NumberWithUnits::operator--(){
  Value--;
  return *this;}

NumberWithUnits NumberWithUnits::operator*(double d) const{
  return NumberWithUnits(Value*d,Unit);}
  
NumberWithUnits ariel::operator*(double d,const NumberWithUnits& b){
  return NumberWithUnits(b.Value*d,b.Unit);}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& b) const{
  return NumberWithUnits(Value+UnitConvert(b.Unit,Unit,b.Value),Unit);}

NumberWithUnits NumberWithUnits::operator+() const{
  return NumberWithUnits{Value,Unit};}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& b) const{
  return NumberWithUnits(Value-UnitConvert(b.Unit,Unit,b.Value),Unit);}

NumberWithUnits NumberWithUnits::operator-() const{
  return NumberWithUnits(-Value,Unit);}

bool ariel::operator==(const NumberWithUnits& a, const NumberWithUnits& b){
  double tempVal = a.Value - a.UnitConvert(b.Unit,a.Unit,b.Value);
  return abs(tempVal)<percision;}

bool ariel::operator!=(const NumberWithUnits& a, const NumberWithUnits& b){
  double tempVal = a.Value - a.UnitConvert(b.Unit,a.Unit,b.Value);
  return !(abs(tempVal)<percision);}

bool ariel::operator<=(const NumberWithUnits&a , const NumberWithUnits& b){
  double tempVal = a.Value - a.UnitConvert(b.Unit,a.Unit,b.Value);
  return abs(tempVal)<percision||tempVal < -percision;}

bool ariel::operator>=(const NumberWithUnits&a , const NumberWithUnits& b){
  double tempVal = a.Value - a.UnitConvert(b.Unit,a.Unit,b.Value);
  return abs(tempVal)<percision||tempVal>percision;}

bool ariel::operator>(const NumberWithUnits& a ,const NumberWithUnits& b){
  double tempVal = a.Value - a.UnitConvert(b.Unit,a.Unit,b.Value);
  return percision<tempVal;}

bool ariel::operator<(const NumberWithUnits& a,const NumberWithUnits& b){
  double tempVal = a.Value - a.UnitConvert(b.Unit,a.Unit,b.Value);
  return -percision>tempVal;}

std::istream& ariel::operator>>(istream& is, NumberWithUnits &a){
  string str;
  char c = ' ';
  while(is.get(c)){
    if(c==']'){break;}
    str+=c;    
  }
  //after deleting spaces, get the values accordingly
  str.erase(remove(str.begin(), str.end(), ' '),str.end());
  a.Value = stod(str.substr(0,str.find('[')));
  a.Unit = (str.substr(str.find('[')+1,str.size()-str.find('[')-1));
  //check if unit exist in table
  if(NumberWithUnits::ConvertionMap.find(a.Unit)==NumberWithUnits::ConvertionMap.end()){
    throw invalid_argument("Measuremeant unit "+ a.Unit + " does not exist or spelled incorrectly.");
  }
  return is;}
  
std::ostream& ariel::operator<<(std::ostream& os ,const NumberWithUnits &a){
  return os << a.Value <<"["<<a.Unit<<"]";
}

