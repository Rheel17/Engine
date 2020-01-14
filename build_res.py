import os
import sys
import re
import mmap


def get_resources():
    files = []

    for r, d, f in os.walk("res"):
        for file in f:
            if not file.endswith("CMakeLists.txt"):
                files.append(os.path.join(r, file))

    return [file for file in files]


def has_new(header, object, resource_files):
    # Find the last modified generated object
    header_mod = os.path.getmtime(header)
    source_mod = os.path.getmtime(object)
    latest_mod = max(header_mod, source_mod)

    # Check if any file was edited after the last generation
    for file in resource_files:
        modified = os.path.getmtime(file)

        if modified > latest_mod:
            return True

    return False


def generate(header_file_out, source_file_out, resource_files):
    # Generate the header file
    header_file_out.write("// THIS IS A MACHINE GENERATED FILE. DO NOT EDIT!\n")
    header_file_out.write("\n")
    header_file_out.write("#ifndef ___RE_RESOURCES_H\n")
    header_file_out.write("#define ___RE_RESOURCES_H\n")
    header_file_out.write("\n")
    header_file_out.write("#include <string>\n")
    header_file_out.write("#include <unordered_map>\n")
    header_file_out.write("\n")
    header_file_out.write("class ___res___ {\n")
    header_file_out.write("\n")
    header_file_out.write("public:\n")
    header_file_out.write("\t___res___() = delete;\n")
    header_file_out.write("\n")
    header_file_out.write("private:\n")

    for file in resource_files:
        header_file_out.write("\tstatic const std::string ___res___")
        header_file_out.write(re.sub("[^\\w\\d]", "_", file[4:]))
        header_file_out.write(";\n")

    header_file_out.write("\n")
    header_file_out.write("\tstatic const std::unordered_map< std::string, const std::string * > _resources;\n")
    header_file_out.write("\n")
    header_file_out.write("public:\n")
    header_file_out.write("\tstatic const std::string& Get(const std::string& resource);\n")
    header_file_out.write("\n")
    header_file_out.write("};\n")
    header_file_out.write("\n")
    header_file_out.write("#endif\n")

    # Generate the source file
    source_file_out.write("// THIS IS A MACHINE GENERATED FILE. DO NOT EDIT!\n")
    source_file_out.write("#pragma clang diagnostic push\n")
    source_file_out.write("#pragma ide diagnostic ignored \"cert-err58-cpp\"\n")
    source_file_out.write("\n")
    source_file_out.write("#include \"_res.h\"\n")
    source_file_out.write("\n")
    source_file_out.write("#include <iostream>\n")
    source_file_out.write("\n")

    for file in resource_files:
        source_file_out.write("const std::string ___res___::___res___")
        source_file_out.write(re.sub("[^\\w\\d]", "_", file[4:]))
        source_file_out.write(" = {\n")
        source_file_out.write("\t\t")

        # read the resource file and put it in the source file.
        count = 0

        with open(file, "rb") as f, mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ) as s:
            for byte in s:
                if count == 32:
                    source_file_out.write("\n\t\t")
                    count = 0

                val = byte[0]

                if val > 127:
                    source_file_out.write(str(val - 256))
                else:
                    source_file_out.write(str(val))

                source_file_out.write(", ")
                count += 1

        source_file_out.write("\n")
        source_file_out.write("};\n")
        source_file_out.write("\n")

    source_file_out.write("const std::unordered_map< std::string, const std::string * > ___res___::_resources = {\n")

    for file in resource_files:
        source_file_out.write("\t\t{ \"")
        source_file_out.write(re.sub("[^\\w\\d]", "_", file[4:]))
        source_file_out.write("\", &___res___")
        source_file_out.write(re.sub("[^\\w\\d]", "_", file[4:]))
        source_file_out.write(" },\n")

    source_file_out.write("};\n")
    source_file_out.write("\n")
    source_file_out.write("const std::string& ___res___::Get(const std::string& resource) {\n")
    source_file_out.write("\tauto iter = _resources.find(resource);\n")
    source_file_out.write("\n")
    source_file_out.write("\tif (iter == _resources.end()) {\n")
    source_file_out.write("\t\tstd::cerr << \"Resource does not exist: \" << resource << std::endl;\n")
    source_file_out.write("\t\tthrow std::exception();\n")
    source_file_out.write("\t}\n")
    source_file_out.write("\n")
    source_file_out.write("\treturn *iter->second;\n")
    source_file_out.write("}\n")


if __name__ == "__main__":
    out_header = str(sys.argv[1])
    out_source = str(sys.argv[2])

    do_generate = not os.path.exists(out_header) or not os.path.exists(out_source)

    res_files = get_resources()

    if not do_generate:
        do_generate = has_new(out_header, out_source, res_files)

    if do_generate:
        header_file = open(out_header, "w+")
        source_file = open(out_source, "w+")

        generate(header_file, source_file, res_files)

        header_file.close()
        source_file.close()
