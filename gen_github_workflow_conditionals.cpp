#include <utility>
#include <vector>
#include <iostream>
#include <stdio.h>

void genSolver(const char* const* args, size_t len, const char* target) {
    if (len < 2) return;
    char x[200];
    printf("#define SOLVER_%s(input) \\\n", target);
    // first up, the singles, these are likely to be chosen first
    printf("    !( \\\n");

    // find our non targets
    bool f = true;
    for (size_t i = 0; i < len; i++) {
        if (args[i] != target) {
            if (f) {
                printf("            strcmp (input, \"- %s\") == 0 \\\n", args[i]);
                f = false;
            }
            else {
                printf("         || strcmp (input, \"- %s\") == 0 \\\n", args[i]);
            }
        }
    }
    // we failed to match against single arguments
    // try to match against permutation arguments
    size_t olen = len;
    f = true;
    // gather all argumments that do not match
    for (size_t len = 2; len <= olen; len++) {
        std::vector<size_t> indicies(len);
        bool terminate = false;
        while (!terminate) {
            // operation here
            std::vector<const char*> values;
            values.reserve(len);
            for (auto index : indicies)
                values.push_back(args[index]);

            std::string msg;
            bool f_ = true;
            char inverser = '!';
            for (auto v : values) {
                if (!f_) msg += ",";
                else f_ = false;
                if (v == target) inverser = ' ';
                msg += v;
            }
            if (inverser == '!') {
                printf("         || strcmp (input, \"- %s\") == 0 \\\n", msg.data());
            }

            // increment indicies
            size_t incrementing_index = len;
            bool reached_completed = true;
            while (incrementing_index-- != 0) {
                if (++indicies[incrementing_index] != olen) {
                    reached_completed = false;
                    break;
                }
                else {
                    indicies[incrementing_index] = 0;
                }
            }
            if (reached_completed) terminate = true;
        }
    }
    printf("     )\n");
}

void genSolver(const char* const* args, size_t len) {
    for (size_t i = 0; i < len; i++) {
        genSolver(args, len, args[i]);
    }
}

void genSolver(const std::vector<const char*>& args) { genSolver(args.data(), args.size()); }


void genSolverTests(const char* const* args, size_t len) {
    if (len < 2) return;
    // for each argument, output a solver
    const char* m_s = "strcmp";
    const char* m_e = " == 0";
    char x[200];
    for (size_t i = 0; i < len; i++) {
        {
            sprintf(x, "- %s", "unknown, should ignore and return true");
            printf("    printf(\"(1xN) ends_%s (\\\"%s\\\") = %%s\\n\", SOLVER_%s(\"%s\") ? \"true\" : \"false\");\n", args[i], x, args[i], x);
        }
        for (size_t i1 = 0; i1 < len; i1++)
        {
            sprintf(x, "- %s", args[i1]);
            printf("    printf(\"(1xN) ends_%s (\\\"%s\\\") = %%s\\n\", SOLVER_%s(\"%s\") ? \"true\" : \"false\");\n", args[i], x, args[i], x);
        }
        size_t olen = len;
        for (size_t len = 2; len <= olen; len++) {
            std::vector<size_t> indicies(len);
            bool terminate = false;
            while (!terminate) {
                // operation here
                std::vector<const char*> values;
                values.reserve(len);
                for (auto index : indicies)
                    values.push_back(args[index]);

                std::string msg = "- ";
                bool f = true;
                for (auto v : values) {
                    if (!f) msg += ",";
                    else f = false;
                    msg += v;
                }

                printf("    printf(\"<%zux%zu> ends_%s (\\\"%s\\\") = %%s\\n\", SOLVER_%s(\"%s\") ? \"true\" : \"false\");\n", len, len, args[i], msg.data(), args[i], msg.data());

                // increment indicies
                size_t incrementing_index = len;
                bool reached_completed = true;
                while (incrementing_index-- != 0) {
                    if (++indicies[incrementing_index] != olen) {
                        reached_completed = false;
                        break;
                    }
                    else {
                        indicies[incrementing_index] = 0;
                    }
                }
                if (reached_completed) terminate = true;
            }
        }
    }
}

void genSolverTests(const std::vector<const char*>& args) { genSolverTests(args.data(), args.size()); }





void genSolverGW(const char* const* args, size_t len, const char* target) {
    if (len < 2) return;
    char x[200];
    printf("# github workflow solver - %s \\\n", target);
    // first up, the singles, these are likely to be chosen first
    printf("    if: ${{ !(");

    // find our non targets
    bool f = true;
    for (size_t i = 0; i < len; i++) {
        if (args[i] != target) {
            if (f) {
                printf("endsWith(github.event.head_commit.message, '- %s')", args[i]);
                f = false;
            }
            else {
                printf(" || endsWith(github.event.head_commit.message, '- %s')", args[i]);
            }
        }
    }
    // we failed to match against single arguments
    // try to match against permutation arguments
    size_t olen = len;
    f = true;
    // gather all argumments that do not match
    for (size_t len = 2; len <= olen; len++) {
        std::vector<size_t> indicies(len);
        bool terminate = false;
        while (!terminate) {
            // operation here
            std::vector<const char*> values;
            values.reserve(len);
            for (auto index : indicies)
                values.push_back(args[index]);

            std::string msg;
            bool f_ = true;
            char inverser = '!';
            for (auto v : values) {
                if (!f_) msg += ",";
                else f_ = false;
                if (v == target) inverser = ' ';
                msg += v;
            }
            if (inverser == '!') {
                printf(" || endsWith(github.event.head_commit.message, '- %s')", msg.data());
            }

            // increment indicies
            size_t incrementing_index = len;
            bool reached_completed = true;
            while (incrementing_index-- != 0) {
                if (++indicies[incrementing_index] != olen) {
                    reached_completed = false;
                    break;
                }
                else {
                    indicies[incrementing_index] = 0;
                }
            }
            if (reached_completed) terminate = true;
        }
    }
    printf(") }}\n");
}

void genSolverGW(const char* const* args, size_t len) {
    for (size_t i = 0; i < len; i++) {
        genSolverGW(args, len, args[i]);
    }
}

void genSolverGW(const std::vector<const char*>& args) { genSolverGW(args.data(), args.size()); }

int main() {
    //printf("#include <string.h>\n");
    //printf("#include <stdio.h>\n");
    //printf("\n");
    std::vector<const char*> vec = { "android", "windows", "linux", "macos" };
    genSolverGW(vec);
    //genSolver(vec);
    //printf("int main() {\n");
    //genSolverTests(vec);
    //printf("}\n");
    return 0;
}
