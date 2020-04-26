import os
import re


def get_files():
    files = []

    for r, d, f in os.walk("src/RheelEngine"):
        for file in f:
            if file.endswith(".h"):
                files.append(re.sub(r"\\", "/", os.path.join(r, file))[16:])

    return files


def get_filters():
    patterns = []

    with open("include_headers.txt") as f:
        for line in f:
            ln = line.strip()
            if not ln:
                continue

            patterns.append(ln)

    return [re.compile(pattern) for pattern in patterns]


def get_included_headers():
    files = get_files()
    filters = get_filters()
    headers = []

    for file in files:
        for rexp in filters:
            if rexp.match(file):
                headers.append(file)

    return headers


if __name__ == "__main__":
    headers = get_included_headers()
    header_string = '\n'.join(["#include \"RheelEngine/" + header + "\"" for header in headers])
    header_string = "// BEGIN GENERATED INCLUDES\n" + header_string + "\n// END GENERATED INCLUDES"

    with open("src/RheelEngine.h") as f:
        src = f.read()
        src_after = re.sub(r"// BEGIN GENERATED INCLUDES.*?// END GENERATED INCLUDES", header_string, src,
                           flags=re.DOTALL)

    if src != src_after:
        with open("src/RheelEngine.h", "w") as f:
            f.write(src_after)
