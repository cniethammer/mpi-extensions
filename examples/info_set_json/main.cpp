// Example: set an MPI info (key, value) pair from the command line via
// MPI_Info_set_json and read it back.
//
// Usage: mpirun -n 2 ./info_set_json_example <key> <value>

#include <mpi.h>
#include <mpi-extensions.h>

#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

// Escape a string so it can be embedded as a JSON string literal; without this,
// a '"' or '\' would break or alter the JSON input.
static std::string json_escape(std::string_view s) {
    std::string out;
    out.reserve(s.size() + 2);
    for (unsigned char c : s) {
        switch (c) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\b': out += "\\b"; break;
        case '\f': out += "\\f"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            // Other control characters (0xXX) must use the generic \u00XX form; all
            // remaining bytes (including UTF-8) are valid in JSON as they are.
            if (c < 0x20) {
                char buf[7];
                std::snprintf(buf, sizeof buf, "\\u%04x", c);
                out += buf;
            } else {
                out += static_cast<char>(c);
            }
        }
    }
    return out;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 3) {
        if (rank == 0)
            std::fprintf(stderr, "Usage: %s <key> <value>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    // Build {"<key>": "<value>"}
    const std::string json =
        "{\"" + json_escape(argv[1]) + "\": \"" + json_escape(argv[2]) + "\"}";

    MPI_Info info;
    MPI_Info_create(&info);

    int err = MPI_Info_set_json(info, json.c_str());
    if (err != MPI_SUCCESS) {
        if (rank == 0)
            std::fprintf(stderr, "MPI_Info_set_json failed for input: %s\n",
                         json.c_str());
        MPI_Info_free(&info);
        MPI_Finalize();
        return 1;
    }

    // Read the value back to show it was stored in the info object.
    int valuelen = 0, flag = 0;
    MPI_Info_get_valuelen(info, argv[1], &valuelen, &flag);
    if (flag && rank == 0) {
        std::vector<char> value(valuelen + 1);
        MPI_Info_get(info, argv[1], valuelen, value.data(), &flag);
        std::printf("JSON input : %s\n", json.c_str());
        std::printf("info[\"%s\"] = \"%s\"\n", argv[1], value.data());
    } else if (!flag && rank == 0) {
        std::fprintf(stderr, "key '%s' not found in info object\n", argv[1]);
    }

    MPI_Info_free(&info);
    MPI_Finalize();
    return flag ? 0 : 1;
}
