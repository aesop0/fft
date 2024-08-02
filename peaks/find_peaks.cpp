#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

/* 
==================================================================================

First prints out all peaks (specifically, upper peaks) and the rows they're found
on, for debugging.

Prints out the average distance between peaks in seconds, and the root-mean-square
of a sine wave signal of f = 60 hz, sampling rate = 2500/sec.

==================================================================================
*/ 



// Truncates points 1-129 and 2001+
void pre_process(std::vector<double> &data) {
    if (data.size() > 2000) {
        data.resize(2000);
    }

    if (data.size() >= 128) {
        data.erase(data.begin(), data.begin() + 128);
    }
}

double calculate_rms(const std::vector<double> &data, size_t first_peak, size_t last_peak){
    
    double rms = 0;
    
    for(size_t i=first_peak; i<last_peak; ++i){
        rms += pow(data[i], 2);
    }
    rms = sqrt( rms / ( last_peak - first_peak + 1 ) );
    
    return rms;
}


double calculate_avg_peak_dist(const std::vector<double> &data, size_t cycles, size_t &first_peak, size_t &last_peak){
    
    unsigned int peak_counter = 0;
    size_t first_peak_index = 0, last_peak_index = 0;

    size_t i = 1;
    while( i < data.size() && peak_counter < cycles+1 ){
        if (data[i-1] < data[i] && data[i+1] < data[i]) { // (If peak found)
            std::cout << "Row " << i+129 << ", " << data[i] << '\n';
            
            if(peak_counter == 0)
                first_peak_index = i;
            else if(peak_counter == cycles)
                last_peak_index = i;
            peak_counter++;

            i += 30;
        }
        else ++i;
    }
    first_peak = first_peak_index;
    last_peak = last_peak_index;

    return (double)(last_peak_index - first_peak_index) / cycles / 2500;

}

int main()
{
    


    // Read input file into a vector   

    std::ifstream input_file("sample_data.txt");
    std::ofstream output_file("output.txt");
    
    if (!input_file.is_open()){
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }
    if (!output_file.is_open()){
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    std::vector<double> data;
    double value;
    while (input_file >> value)
    {
        data.push_back(value);
    }
    input_file.close();


    pre_process(data);


    // Calculate, print average peak distance over 20 cycles

    std::cout << '\n';
    size_t first_peak = 0, last_peak = 0;
    double avg_dist = calculate_avg_peak_dist(data, 20, first_peak, last_peak);
    std::cout << "\n\n\nAverage peak distance: " << avg_dist << " seconds\n\n";

    // Calculate, print RMS

    std::cout << "RMS = " << calculate_rms(data, first_peak, last_peak) << "\n\n\n";

    return 0;

}





