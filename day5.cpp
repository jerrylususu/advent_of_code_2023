#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>

std::vector<std::pair<long long, long long>> true_seed_ranges;

std::vector<std::tuple<long long, long long, long long>> seed_to_soil_list;
std::vector<std::tuple<long long, long long, long long>> soil_to_fertilizer_list;
std::vector<std::tuple<long long, long long, long long>> fertilizer_to_water_list;
std::vector<std::tuple<long long, long long, long long>> water_to_light_list;
std::vector<std::tuple<long long, long long, long long>> light_to_temperature_list;
std::vector<std::tuple<long long, long long, long long>> temperature_to_humidity_list;
std::vector<std::tuple<long long, long long, long long>> humidity_to_location_list;

std::vector<std::tuple<long long, long long, long long>>* current_list = nullptr;

void parseInputFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        } else if (line.find("seeds") != std::string::npos) {
            std::cout << "line" << line << std::endl;
            std::string seeds_str = line.substr(7);
            std::cout << "seed str" << seeds_str << std::endl;
            std::istringstream iss(seeds_str);
            long long seed;
            std::vector<long long> seeds;
            while (iss >> seed) {
                std::cout << "seed" << seed << std::endl;
                seeds.push_back(seed);
            }
            true_seed_ranges.clear();
            for (size_t i = 0; i < seeds.size(); i += 2) {
                true_seed_ranges.emplace_back(seeds[i], seeds[i + 1]);
            }
        } else if (line.find("seed-to-soil map") != std::string::npos) {
            current_list = &seed_to_soil_list;
        } else if (line.find("soil-to-fertilizer map") != std::string::npos) {
            current_list = &soil_to_fertilizer_list;
        } else if (line.find("fertilizer-to-water map") != std::string::npos) {
            current_list = &fertilizer_to_water_list;
        } else if (line.find("water-to-light map") != std::string::npos) {
            current_list = &water_to_light_list;
        } else if (line.find("light-to-temperature map") != std::string::npos) {
            current_list = &light_to_temperature_list;
        } else if (line.find("temperature-to-humidity map") != std::string::npos) {
            current_list = &temperature_to_humidity_list;
        } else if (line.find("humidity-to-location map") != std::string::npos) {
            current_list = &humidity_to_location_list;
        } else {
            long long target_start, source_start, length;
            std::istringstream iss(line);
            iss >> target_start >> source_start >> length;
            current_list->emplace_back(source_start, target_start, length);
        }
    }
}

void sortRanges() {
    std::sort(seed_to_soil_list.begin(), seed_to_soil_list.end(), [](const std::tuple<long long, long long, long long>& a, const std::tuple<long long, long long, long long>& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    std::sort(soil_to_fertilizer_list.begin(), soil_to_fertilizer_list.end(), [](const std::tuple<long long, long long, long long>& a, const std::tuple<long long, long long, long long>& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    std::sort(fertilizer_to_water_list.begin(), fertilizer_to_water_list.end(), [](const std::tuple<long long, long long, long long>& a, const std::tuple<long long, long long, long long>& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    std::sort(water_to_light_list.begin(), water_to_light_list.end(), [](const std::tuple<long long, long long, long long>& a, const std::tuple<long long, long long, long long>& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    std::sort(light_to_temperature_list.begin(), light_to_temperature_list.end(), [](const std::tuple<long long, long long, long long>& a, const std::tuple<long long, long long, long long>& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    std::sort(temperature_to_humidity_list.begin(), temperature_to_humidity_list.end(), [](const std::tuple<long long, long long, long long>& a, const std::tuple<long long, long long, long long>& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    std::sort(humidity_to_location_list.begin(), humidity_to_location_list.end(), [](const std::tuple<long long, long long, long long>& a, const std::tuple<long long, long long, long long>& b) {
        return std::get<0>(a) < std::get<0>(b);
    });

}

long long findTarget(long long input_value, const std::vector<std::tuple<long long, long long, long long>>& range_tuple_list) {
    // now ranges are sorted by source start, we can use binary search
    int left = 0;
    int right = range_tuple_list.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        long long source_start, target_start, length;
        std::tie(source_start, target_start, length) = range_tuple_list[mid];

        if (input_value >= source_start && input_value < source_start + length) {
            return target_start + (input_value - source_start);
        }

        if (input_value < source_start) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return input_value;
}

long long findLocationFromSeed(long long seed) {
    long long soil = findTarget(seed, seed_to_soil_list);
    long long fertilizer = findTarget(soil, soil_to_fertilizer_list);
    long long water = findTarget(fertilizer, fertilizer_to_water_list);
    long long light = findTarget(water, water_to_light_list);
    long long temperature = findTarget(light, light_to_temperature_list);
    long long humidity = findTarget(temperature, temperature_to_humidity_list);
    return findTarget(humidity, humidity_to_location_list);
}

int main(int argc, char* argv[]) {
    std::string filename;
    if (argc < 2) {
        std::cout << "Please provide the input file name as a command line argument." << std::endl;
        return 1;
    } else {
        filename = argv[1];
    }

    parseInputFile(filename);

    sortRanges();

    long long min_location = 1000000000000000000;
    long long total_range = 0;
    long long scanned_range = 0;

    // Calculate the total range
    for (const auto& seed : true_seed_ranges) {
        total_range += seed.second;
    }

    // prlong long all true seed ranges
    std::cout << "true_seed_ranges: ";
    for (const auto& seed : true_seed_ranges) {
        std::cout << "(" << seed.first << ", " << seed.second << ") ";
    }
    std::cout << std::endl;

    for (const auto& seed : true_seed_ranges) {
        // current in range
        std::cout << "seed: " << seed.first << " " << seed.second << std::endl;
        for (long long seed_value = seed.first; seed_value < seed.first + seed.second; ++seed_value) {
            scanned_range++;
            if (scanned_range % 10000 == 0) {
                double percentage = static_cast<double>(scanned_range) / total_range * 100;
                std::cout << "Scanned range: " << scanned_range << " Percentage: " << percentage << "%" << std::endl;
            }

            long long loc = findLocationFromSeed(seed_value);
            if (loc < min_location) {
                min_location = loc;
            }
        }
    }

    std::cout << "min location: " << min_location << std::endl;

    return 0;
}

