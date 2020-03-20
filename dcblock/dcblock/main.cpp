//
//  main.cpp
//  dcblock
//
//  Created by syfo_dias on 20/03/2020.
//  Copyright © 2020 syfo_dias. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

// test vectors
static vector<double> ramp{0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

const vector<double> fillRect(const int size)
{
    static vector<double> rect;
    for(int i = 0; i < size; ++i)
    {
        auto val = (i < 5) ? 0 : 1;
        rect.push_back(val);
    }
    return rect;
}

const vector<double> fillDC(const int size, double level)
{
    static vector<double> dc;
    for(int i = 0; i < size; ++i) dc.push_back(level);
    return dc;
}

// simple HPF for DC offset filtering
template<typename T>
class DCblock
{
public:
    DCblock()
    :x(0),
    y(0)
    {
    }
    
    T process_pole_zero(const T& input)
    {
        
        T outpt = input - x + y * pole;
        x = input;
        y = outpt;
        return outpt;
    }
    

private:
    static constexpr T pole{0.995};
    T x;
    T y;
    
    
    
};


// test function
void test_DCblock(vector<double>& input, const char* indicator)
{
    if(input.size() < 1) throw runtime_error("input vector is empty");
    if(input.size() > 9800) throw runtime_error("input vector is too large");
    
    DCblock<double> testblock;
    
    vector<double> output_buffer;
    for(int i = 0; i < input.size(); ++i)
    {
        auto sample = testblock.process_pole_zero(input[i]);
        output_buffer.push_back(sample);
    }
    
    cout << indicator << " through DC Block:\n ";
    
    if(input.size() != output_buffer.size()) throw runtime_error("output buffer size does not match input buffer size");
    
    for(int j = 0; j < output_buffer.size(); ++j)
    {
        if(j%3 == 0) cout << '\n';
        cout << output_buffer[j] << '\t';
    }
    
    
}



int main(int argc, const char * argv[]) {
    try {
        
        test_DCblock(ramp, "ramp");
        vector<double> rect = fillRect(10);
        test_DCblock(rect, "rect");
        vector<double> dc = fillDC(9200, 0.5);
        test_DCblock(dc, "dc");
        
    } catch (exception& e) {
        cerr << e.what() << '\n';
        return 1;
    }
    catch(...)
    {
        cerr << "opps\n";
        return 2;
    }
    return 0;
}
