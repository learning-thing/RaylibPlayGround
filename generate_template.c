//
// Created by benji on 14.06.26.
//
#include <stdio.h>
#include <stdlib.h>

#define start \
"#ifndef RAYLIBPLAYGROUND_TEMPLATES_HPP\n" \
"#define RAYLIBPLAYGROUND_TEMPLATES_HPP\n\n" \
"#include <iostream>\n\n" \
"inline const char *dts = \n"

#define scriptStart \
inline const char *script_template_src =

#define end \
"\n\n// later create a template\n" \
"inline void create_script_template(const char *projectName) {\n" \
"    std::ofstream script(projectName);\n" \
"    script << script_template_src;\n" \
"}\n" \
"inline void create_dts_template() {\n" \
"    std::ofstream dts_file(\".d.ts\");\n" \
"    dts_file << dts;\n" \
"}\n" \
"#endif //RAYLIBPLAYGROUND_TEMPLATES_HPP"


char *readFile(const char *path, size_t *size) {
    FILE *file = fopen(path, "r");
    if (!file) return nullptr;
    fseek(file, 0, SEEK_END);
    const size_t pos = ftell(file);
    *size = pos;
    printf("Size: %lu\n", pos);
    char *buf = malloc(*size);
    fseek(file, 0, 0);
    fread(buf, sizeof(char), *size, file);
    return buf;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("USAGE:\n%s [.d.ts] [script.js] output_header.hpp\n", argv[0]);
        return 1;
    }

    size_t dts_size;
    size_t script_size;
    const char *dts = readFile(argv[1], &dts_size);
    const char *script = readFile(argv[2], &script_size);

    if (!dts || !script) return 1;

    //printf("dts:\n%s\n\n\n", dts);
    //printf("script:\n%s\n\n\n", script);

    FILE *outputHeader = fopen(argv[3], "w");
    // Write dts
    fwrite(start, sizeof(char), sizeof(start)-1, outputHeader);
    const char *newline = "\\n\"\n\"";
    const char *strEnd = "\";";
    for (int i = 0; i < dts_size; ++i) {
        //End of string
        if (i == dts_size-1) {
            fwrite(strEnd, sizeof(char), 2, outputHeader);
            break;
        }
        if (dts[i] == '\n') {
            fwrite(newline, sizeof(char), 5, outputHeader);
        } else {
            fwrite(&dts[i], sizeof(char), 1, outputHeader);
        }
    }



    //Write the end
    fwrite(end, sizeof(char), sizeof(end)-1, outputHeader);
    return 0;
}