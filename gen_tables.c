
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// this program generates:
//
//   tables.cpp
//
// there should be no reason to compile and run this unless you want to change some of the parameters.
// but if you do, do it like this..
//
//   gcc gen_tables.c -o gen_tables
//   ./gentables > tables.cpp
//

#define SID_ARRAY_NAME      "sid_frequency"
#define SUB_SEMITONES    16.0
#define INDENT              "    "
#define VALUES_PER_LINE  8

#define SID_SCALER 985248.0
//#define SID_SCALER 1022727.0 // 1022727 for 6567R8 VIC 6567R56A

#define SINE_ARRAY_NAME  "sin_table"
#define RANGE_X 256 // * 4
#define RANGE_Y 128 // +/-128
#define PI  3.14159265359

uint16_t getSidFrequency(float note) {
    float freq = 8.1758 * powf(2,((note < 0 ? 0 : note)/12.0));
    float result = (16777216.0 / SID_SCALER) * freq; 
    return result > 65535.0 ? 0xffff : (uint16_t)result;
}

void sidTable() {
    int col=-1;
    int count=0;
    printf("\r\n\r\n#include <avr/pgmspace.h>\r\n\r\nconst uint16_t %s[] PROGMEM = {\r\n", SID_ARRAY_NAME);
    printf(INDENT);
    for (float x=0;x<128;x+=(1.0/SUB_SEMITONES)) {
        int c = getSidFrequency(x);
        if (count != 0) {
            printf(",");
        }
        count++;
        if (++col >= VALUES_PER_LINE) {
            printf("\r\n");
            printf(INDENT);
            col = 0;
        }
        printf("0x%04x", c);
    }
    printf("\r\n};\r\n\r\n\r\n");

    printf("// bytes %d\n", count*2);
}

void sineTable() {
    int col=-1;
    int count=0;
    printf("\r\n\r\nconst uint8_t %s[] PROGMEM = {\r\n", SINE_ARRAY_NAME);
    printf(INDENT);
    
    double rr = PI / 2.0;
    
    for (int x=0;x<RANGE_X*4;x++) {
        double r = (rr/RANGE_X)*x;
        uint8_t c = (int)(sin(r) * RANGE_Y);
        if (count != 0) {
            printf(",");
        }
        count++;
        if (++col >= VALUES_PER_LINE) {
            printf("\r\n");
            printf(INDENT);
            col = 0;
        }
        printf("0x%02x", c);
    }
    printf("\r\n};\r\n\r\n\r\n");

    printf("// bytes %d\n", count);
}

int main(int argc, char *argv[]) {
  sidTable();
  sineTable();
}

