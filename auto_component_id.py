import os
import re

new_id = 0


def handle_lines(header_lines):
    global new_id

    idx = 0
    found = False

    for line in header_lines:
        idx += 1
        if re.match(r"\s*//\s*gen_component_id\s*", line):
            found = True
            break

    if not found:
        return

    if not re.match(r"\s*static constexpr const ComponentId id = \d+;\s*", header_lines[idx]):
        header_lines.insert(idx, "")

    old_id_line = header_lines[idx]

    header_lines[idx] = \
        re.sub(r"(\s*)//.*", r"\1static constexpr const ComponentId id = " + str(new_id) + ";", header_lines[idx - 1])
    new_id = new_id + 1

    return old_id_line != header_lines[idx]


def handle_file(header_file):
    with open(header_file) as open_file:
        lines = [line for line in open_file]

    if handle_lines(lines):
        with open(header_file, "w") as open_file:
            for line in lines:
                open_file.write(line)


if __name__ == "__main__":
    for r, d, f in os.walk("src/RheelEngine/Components"):
        for file in f:
            if file.endswith(".h"):
                handle_file(os.path.join(r, file))
