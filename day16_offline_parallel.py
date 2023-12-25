import sys


with open("day16.txt", "r") as f:
    data = f.read()
    
lines = data.split("\n")
pattern = []
for line in lines:
    row = []
    for char in line:
        row.append(char)
    pattern.append(row)
    
def get_next_beams(beam_row, beam_col, direction, char):
    if char == '.':
        # keep going
        if direction == "right":
            return [(beam_row, beam_col + 1, direction)]
        elif direction == "left":
            return [(beam_row, beam_col - 1, direction)]
        elif direction == "up":
            return [(beam_row - 1, beam_col, direction)]
        elif direction == "down":
            return [(beam_row + 1, beam_col, direction)]
    elif char == '/':
        # need do reflect
        if direction == "right":
            return [(beam_row - 1, beam_col, "up")]
        elif direction == "left":
            return [(beam_row + 1, beam_col, "down")]
        elif direction == "up":
            return [(beam_row, beam_col + 1, "right")]
        elif direction == "down":
            return [(beam_row, beam_col - 1, "left")]
    elif char == '\\':
        # also reflect
        if direction == "right":
            return [(beam_row + 1, beam_col, "down")]
        elif direction == "left":
            return [(beam_row - 1, beam_col, "up")]
        elif direction == "up":
            return [(beam_row, beam_col - 1, "left")]
        elif direction == "down":
            return [(beam_row, beam_col + 1, "right")]
    elif char == '|':
        # point side: keep going
        # flat side: split
        if direction == "right" or direction == "left":
            return [(beam_row + 1, beam_col, "down"), (beam_row - 1, beam_col, "up")]
        elif direction == 'up':
            return [(beam_row - 1, beam_col, "up")]
        elif direction == 'down':
            return [(beam_row + 1, beam_col, "down")]
    elif char == '-':
        if direction == "up" or direction == "down":
            return [(beam_row, beam_col + 1, "right"), (beam_row, beam_col - 1, "left")]
        elif direction == 'right':
            return [(beam_row, beam_col + 1, "right")]
        elif direction == 'left':
            return [(beam_row, beam_col - 1, "left")]
    raise Exception("Unknown char: " + char)

def get_next_position(beam_row, beam_col, direction):
    if direction == "right":
        return (beam_row, beam_col + 1)
    elif direction == "left":
        return (beam_row, beam_col - 1)
    elif direction == "up":
        return (beam_row - 1, beam_col)
    elif direction == "down":
        return (beam_row + 1, beam_col)

def count_number_of_non_zero(energized):
    count = 0
    for row in range(len(energized)):
        for col in range(len(energized[row])):
            if energized[row][col] != 0:
                count += 1
    return count


edge_start_beams = []
# top
if sys.argv[1] == "0":
    for idx in range(len(pattern[0])):
        edge_start_beams.append((0, idx, "down"))
# bottom
elif sys.argv[1] == "1":
    for idx in range(len(pattern[-1])):
        edge_start_beams.append((len(pattern) - 1, idx, "up"))
# left
elif sys.argv[1] == "2":
    for idx in range(len(pattern)):
        edge_start_beams.append((idx, 0, "right"))
# right
elif sys.argv[1] == "3":
    for idx in range(len(pattern)):
        edge_start_beams.append((idx, len(pattern[0]) - 1, "left"))
    

edge_energized = []

for outer_idx, edge_start_beam in enumerate(edge_start_beams):
    print("calculating edge: ", outer_idx, edge_start_beam)
    energized = []
    for row in range(len(pattern)):
        energized_row = []
        for col in range(len(pattern[row])):
            energized_row.append(0)
        energized.append(energized_row)

    beams = [edge_start_beam]
    traced_beams = []
    sim_step = 0
    current_energized = 0

    while len(beams) > 0 and sim_step <= 100000:
        sim_step += 1
        last_energized = current_energized
        current_beam = beams.pop(0)
        beam_row, beam_col, direction = current_beam
        if sim_step % 1000 == 0:
            print("step: " + str(sim_step) + " beam: " + str(current_beam) + " direction: " + direction, "len beam", len(beams))
        # print("step: " + str(sim_step) + " beam: " + str(current_beam) + " direction: " + direction, "len beam", len(beams))
        # out of border, ignore
        if beam_row < 0 or beam_row >= len(pattern):
            # print("out of border", beam_row, beam_col, direction)
            continue
        if beam_col < 0 or beam_col >= len(pattern[beam_row]):
            # print("out of border", beam_row, beam_col, direction)
            continue
        
        energized[beam_row][beam_col] += 1

        next_char = pattern[beam_row][beam_col]
        next_beams = get_next_beams(beam_row, beam_col, direction, next_char)
        # print("next beams: " + str(next_beams))

        for next_beam in next_beams:
            if next_beam in traced_beams:
                continue
            else:
                traced_beams.append(next_beam)
                beams.append(next_beam)

        current_energized = count_number_of_non_zero(energized)
        # print("current energized: " + str(current_energized), "last energized: " + str(last_energized))
    
    edge_energized.append(current_energized)
    print("calculating edge: ", outer_idx, edge_start_beam, "done, energized=", current_energized)

print(edge_energized)
print("max", max(edge_energized))