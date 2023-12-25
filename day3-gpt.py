def is_valid_position(row, col, max_row, max_col):
    return 0 <= row < max_row and 0 <= col < max_col

def find_adjacent_digits(schematic, row, col):
    adjacent_digits = []
    max_row = len(schematic)
    max_col = len(schematic[0])
    for i in range(row-1, row+2):
        for j in range(col-1, col+2):
            if is_valid_position(i, j, max_row, max_col) and schematic[i][j].isdigit():
                adjacent_digits.append((i, j))
    return adjacent_digits

def extract_number(schematic, start_row, start_col, visited):
    number = ""
    stack = [(start_row, start_col)]
    while stack:
        current_row, current_col = stack.pop()
        if (current_row, current_col) not in visited:
            visited.add((current_row, current_col))
            number += schematic[current_row][current_col]
            for i, j in find_adjacent_digits(schematic, current_row, current_col):
                stack.append((i, j))
    return int(number)

def sum_part_numbers(schematic):
    total = 0
    visited = set()
    for i in range(len(schematic)):
        for j in range(len(schematic[0])):
            if schematic[i][j].isdigit() and (i, j) not in visited:
                total += extract_number(schematic, i, j, visited)
    return total

engine_schematic = [
    "467..114..",
    "...*......",
    "..35..633.",
    "......#...",
    "617*......",
    ".....+.58.",
    "..592.....",
    "......755.",
    "...$.*....",
    ".664.598.."
]

print(sum_part_numbers(engine_schematic))