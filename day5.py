with open("day5.txt", "r") as f:
    data = f.read()


# soil-to-fertilizer map:
# 50 98 2
# fertilizer-to-water map:
# water-to-light map:
# light-to-temperature map:
# temperature-to-humidity map:
# humidity-to-location map:

true_seed_ranges = []

seed_to_soil_list = []
soil_to_fertilizer_list = []
fertilizer_to_water_list = []
water_to_light_list = []
light_to_temperature_list = []
temperature_to_humidity_list = []
humidity_to_location_list = []

current_list = None

for line in data.split("\n"):
    if line.strip() == "":
        continue
    elif line.startswith("seeds"):
        # parse seeds pairs (two value is a start and a length)
        seeds_str = line.replace("seeds: ", "")
        seeds = [int(i) for i in seeds_str.split(" ")]

        true_seed_ranges = list(zip(seeds[::2], seeds[1::2]))
        # for seed_start, seed_length in seed_pairs:
        #     for i in range(seed_start, seed_start + seed_length):
        #         true_seeds.append(i)
        
    elif line.startswith("seed-to-soil map"):
        current_list = seed_to_soil_list
        continue
    elif line.startswith("soil-to-fertilizer map"):
        current_list = soil_to_fertilizer_list
        continue
    elif line.startswith("fertilizer-to-water map"):
        current_list = fertilizer_to_water_list
        continue
    elif line.startswith("water-to-light map"):
        current_list = water_to_light_list
        continue
    elif line.startswith("light-to-temperature map"):
        current_list = light_to_temperature_list
        continue
    elif line.startswith("temperature-to-humidity map"):
        current_list = temperature_to_humidity_list
        continue
    elif line.startswith("humidity-to-location map"):
        current_list = humidity_to_location_list
        continue
    else:
        # pure mapping
        target_start, source_start, length = line.split(" ")
        source_start = int(source_start)
        target_start = int(target_start)
        length = int(length)
        current_list.append((source_start, target_start, length))
    
def find_target(input_value, range_tuple_list):
    for source_start, target_start, length in range_tuple_list:
        if input_value >= source_start and input_value < source_start + length:
            return target_start + (input_value - source_start)
    # not mapped just keep the same
    return input_value

def find_location_from_seed(seed):
    # print("seed", seed)
    soil = find_target(seed, seed_to_soil_list)
    # print("soil", soil)
    fertilizer = find_target(soil, soil_to_fertilizer_list)
    # print("fertilizer", fertilizer)
    water = find_target(fertilizer, fertilizer_to_water_list)
    # print("water", water)
    light = find_target(water, water_to_light_list)
    # print("light", light)
    temperature = find_target(light, light_to_temperature_list)
    # print("temperature", temperature)
    humidity = find_target(temperature, temperature_to_humidity_list)
    # print("humidity", humidity)
    location = find_target(humidity, humidity_to_location_list)
    # print("location", location)
    return location

# from tqdm import tqdm

min_location = 1000000000000000000000

# for seed in tqdm(true_seed_ranges, desc="Outer Loop"):
#     for seed_value in tqdm(range(seed[0], seed[0] + seed[1]), desc="Inner Loop"):
#         loc = find_location_from_seed(seed_value)
#         if loc < min_location:
#             min_location = loc

for seed in true_seed_ranges:
    print("testing seed ranges", seed)
    for seed_value in range(seed[0], seed[0] + seed[1]):
        loc = find_location_from_seed(seed_value)
        if loc < min_location:
            min_location = loc

print("min location: {}".format(min_location))