#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void process_line(char*, FILE*);
void title_case(char*);
void print_reverse_form(char*, FILE*);
void process_file(FILE*, FILE*);

// in the future, I want to make this application
// pip data in and out through stdio so it can be
// utilized with `cat` and the optionally be piped
// into a file

int main(int argc, char* argv[]) {
    // Rewriter: usage `rewrite [file]`
    // rewrite lines of text file given by the following rules
    // START (space) OTHER STUFF -> Other Stuff (space) Start
    // (lowercase all alphabetical charecters except those beginging a token)
    // eg: "CHAMBERS JOHN B JR & SHERRY R" as "John B Jr & Sherry R Chembers"
    
    // parse args
    char* read_filename = argv[1];
    
    // open file for r/w
    FILE* fp_read;
    fp_read = fopen(read_filename, "r");
    if(fp_read == NULL) {
        fprintf(stderr, "could not open ");
        fprintf(stderr, "%s", read_filename);
        exit(EXIT_FAILURE);
    }
    
    char* write_filename = strcat(read_filename, "_processed");
    FILE* fp_write = stdout;//fopen(write_filename, "w");
    if(fp_write == NULL || ferror(fp_write)) {
        fprintf(stderr, "could not create %s", write_filename);
        exit(EXIT_FAILURE);
    }
    
    // read line by line
    char* line;
    size_t len = 0;
    size_t read_c;
    while(getline(&line, &len, fp_read) != -1) {
        //write formatted line
        //fprintf(stderr, "next line\n");
        process_line(line, fp_write);
    }
    //fprintf(stderr, "done\n");
    
    // close resources
    fclose(fp_read);
    fclose(fp_write);
    //printf("\nrewriter saved to %s", write_filename);
    exit(EXIT_SUCCESS);
}

void process_line(char* line, FILE* fp_write) {
    title_case(line);
    print_reverse_form(line, fp_write);
}

void title_case(char* line) {
    //walk along line making alpha chars capital or lowercase appropriately
    int next_capital = 1;
    int line_len = strlen(line);
    for(int i = 0; i < line_len; i++) {
        if(isalpha(line[i])) {
            if(next_capital == 1) {
                next_capital = 0;
                line[i] = toupper(line[i]); //uppercase
            }
            else {
                line[i] = tolower(line[i]); //lowercase
            }
        }
        else if(line[i] == ' ') {
            next_capital = 1;
        }
    }
}

void print_reverse_form(char* line, FILE* fp_write) {
    // first print address
    while(*line != ' ') {
        fputc(*line, fp_write);
        ++line;
    }
    
    // skip space in between address and lastname
    ++line;
    
    // get pointer starting at next space which the proceeds
    // to the rest of the string
    char* rest = strchr(line, ' ');
    while(*rest != '\n') {
        fputc(*rest, fp_write);
        ++rest;
    }
    
    // write space in between first part and last name
    fputc(' ', fp_write);
    
    // print last name
    // which is line until next space encountered
    while(*line != ' ') {
        fputc(*line, fp_write);
        ++line;
    }
    
    // print newline
    fputc('\n', fp_write);
}
