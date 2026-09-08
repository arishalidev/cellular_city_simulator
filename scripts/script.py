import csv

block_size = 12
cell_map = []
filename = "../region.csv"
row_size = block_size * 2 + 1

def generate_cell_map():
    i = 0
    for row in range(block_size):
        cell_row = []

        for col in range(block_size):
            cell_row.append("R")

        cell_row.append("T")

        for col in range(block_size):
            cell_row.append("I")

        cell_map.append(cell_row)

    cell_map.append(generate_power_lines())

    for row in range(block_size):
        cell_row = []

        for col in range(block_size):
            cell_row.append("C")

        cell_row.append("T")

        for col in range(block_size):
            cell_row.append("R")

        cell_map.append(cell_row)

    cell_map.append(generate_power_lines())


def generate_power_lines():
    cell_row = []
    for cell in range(row_size):
        cell_row.append("T")
    return cell_row

if __name__ == '__main__':
    generate_cell_map()

    with open(filename, mode="w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)

        writer.writerow(generate_power_lines())
        writer.writerows(cell_map)

    print("Generated region map with total size of " +  str(row_size) + " by " + str(row_size) + " and a total cell count of " + str(row_size * (row_size + 2)))


