import os


def handle_lines(header_lines):
    pass


def handle_file(header_file):
    lines = []

    with open(header_file) as open_file:
        lines = [line for line in open_file]

    handle_lines(lines)

    with open(header_file, "w") as open_file:
        for line in lines:
            open_file.write(line)


if __name__ == "__main__":
    for r, d, f in os.walk("src/RheelEngine/Components"):
        for file in f:
            if file.endswith(".h"):
                handle_file(os.path.join(r, file))
