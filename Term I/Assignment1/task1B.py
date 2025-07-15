import csv


def read_csv(filename):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        header = next(reader)
        return (header, list(reader))


def write_csv(data, header, filename="out.csv"):
    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(header)
        for i in data:
            writer.writerow(i)


def main():
    part2 = read_csv("src/carddump2.csv")
    part1 = read_csv("src/carddump1.csv")

    def fexp(x):
        m, y = x[1].split("/")
        return int(y) * 100 + int(m)

    fpin = lambda x: int(x[3])

    sortedbyPin = sorted(part2[1], key=fpin)
    sortedbyExp = sorted(sortedbyPin, key=fexp)

    for i in range(len(sortedbyExp)):
        sortedbyExp[i][0] = sortedbyExp[i][0].replace(
            "****-****-****", part1[1][i][0][:14]
        )

    write_csv(sortedbyExp, part2[0], "results/sorted_cards.csv")

    print("New results/sorted_card.svg file created")


main()
