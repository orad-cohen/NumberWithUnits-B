#include<unordered_map>

#include <fstream>


namespace ariel{
    
    class NumberWithUnits{
    private:
        std::string Unit;        
        double Value;
        static std::unordered_map<std::string,std::unordered_map<std::string,double>> ConvertionMap;
        static void InsertUpdate(const std::string & val1,const std::string &val2, double mult);
        static double UnitConvert(const std::string&,const std::string&,double);
       
    public:
    
    
    NumberWithUnits(double,const std::string&);
    static void read_units(std::ifstream&);


    const NumberWithUnits& operator-=(const NumberWithUnits&) ;
    const NumberWithUnits& operator+=(const NumberWithUnits&) ;

    NumberWithUnits operator ++(int) ;
    NumberWithUnits& operator++();

    NumberWithUnits operator --(int) ;
    NumberWithUnits& operator--();

    friend std::istream& operator>>(std::istream&, NumberWithUnits&);
    friend std::ostream& operator<<(std::ostream&, const NumberWithUnits&);

    NumberWithUnits operator*(double) const;
    friend NumberWithUnits operator*(double, const NumberWithUnits&);

    NumberWithUnits operator+(const NumberWithUnits&) const;
    NumberWithUnits operator+() const;

    NumberWithUnits operator-(const NumberWithUnits&) const;
    NumberWithUnits operator-() const;

    friend bool operator!=(const NumberWithUnits&,const NumberWithUnits&);
    friend bool operator==(const NumberWithUnits&,const NumberWithUnits&);
    friend bool operator<=(const NumberWithUnits&, const NumberWithUnits&);
    friend bool operator>=(const NumberWithUnits&, const NumberWithUnits&);
    friend bool operator>(const NumberWithUnits&,const NumberWithUnits&);
    friend bool operator<(const NumberWithUnits&,const NumberWithUnits&);

};






}

