#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

int my_printf(char* format, ...);
char* convert_i(int value, int base);
char* convert_ui(unsigned int value, int base);
char* convert_p(void* pointer);

int main()
{
    // my_printf("char %c, string %s, integer %d, unsigned integer %u, octal %o, hexadecimal %x, pointer %p", 't', "hello", -123, -123, -123, -123, "NULL");
    my_printf("x%ux\n", 0);
    return 0;
}

char* convert_i(int value, int base)
{
    int size = 0;
    char* result;
    int is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value = value*(-1);
    }
    int value_copy = value;
    if(value == 0){
        result = malloc(2*sizeof(char));
        result[0] = 48;
        result[1] = 0;
        return result;
    }
    while(value != 0) {
        value = value / base;
        size++;
    }
    result = malloc((is_negative + size + 1)*sizeof(char));
    for(int i = size + is_negative - 1; i >= is_negative; i--) {
        result[i] = value_copy % base + 48;
        value_copy = value_copy / base;
        if(is_negative == 1) {
            result[0] = 45;
        }
    }
    result[size + is_negative] = 0;  
    return result;
}

char* convert_ui(unsigned int value, int base)
{
    int size = 0;
    char* result;
    unsigned int value_copy = value;
    if(value == 0){
        result = malloc(2*sizeof(char));
        result[0] = 48;
        result[1] = 0;
        return result;
    }
    while(value != 0) {
        value = value / base;
        size++;
    }
    result = malloc((size + 1)*sizeof(char));
    for(int i = size - 1; i >= 0; i--) {
        if (value_copy % base > 9) {
            result[i] = value_copy % base + 87;
            value_copy = value_copy / base;
        }
        else {
            result[i] = value_copy % base + 48;
            value_copy = value_copy / base;
        }
    }

    result[size] = 0;
    return result;
}

char* convert_p(void* pointer)
{
    unsigned long ui = (unsigned long)pointer;
    char* hexa = convert_ui(ui, 16);
    char* result = malloc((2 + strlen(hexa) + 1)*sizeof(char));
    result[0] = 0;
    if(strlen(hexa) == 0) {
        result = realloc(result, 6);
        result = strcpy(result, "(nil)");
    }
    else {
        result = strcat(result, "0x");
        result = strcat(result, hexa);
    }
    result[strlen(hexa)] = 0;
    free(hexa);
    return result;
}

int my_printf(char *format, ...)
{
    int size = 0;
    char* temp;
    va_list ap;
    va_start (ap, format);

    int i;
    unsigned int ui;
    char* s;
    void* pointer;
    char* error = "(nil)";

    for (temp = format; *temp != '\0'; temp++) {

        while (*temp != '%') {
            write(1, temp, 1);
            size++;
            temp++;
            if(*temp == 0) {
                break;
            }
        }
        if(*temp == 0) {
            break;
        }
        temp++;
        switch(*temp) {

            case 'd':
            i = va_arg(ap, int);
            char* res_i = convert_i(i, 10);
            write(1, res_i, strlen(res_i));
            size += strlen(res_i);
            free(res_i);
            break;

            case 'u':
            ui = va_arg(ap, unsigned int);
            char* res_ui = convert_ui(ui, 10);
            write(1, res_ui, strlen(res_ui));
            size += strlen(res_ui);
            free(res_ui);
            break;

            case 'o':
            ui = va_arg(ap, unsigned int);
            char* res_o = convert_ui(ui, 8);
            write(1, res_o, strlen(res_o));
            size += strlen(res_o);
            free(res_o);
            break;

            case 'x':
            ui = va_arg(ap, unsigned int);
            char* res_x = convert_ui(ui, 16);
            write(1, res_x, strlen(res_x));
            size += strlen(res_x);
            free(res_x);
            break;

            case 'c':
            i = va_arg(ap, int);
            write(1, &i, 1);
            size++;
            break;

            case 's':
            s = va_arg(ap, char*);
            if(s == NULL) {
                write(1, error, strlen(error));
                size += strlen(error);
            }
            else {
                write(1, s, strlen(s));
                size += strlen(s);
            }
            
            break;

            case 'p':
            pointer = va_arg(ap, void*);
            char* res_p = convert_p(pointer);
            write(1, res_p, strlen(res_p));
            size += strlen(res_p);
            free(res_p);
            break;
        }
    }    
    va_end(ap);
    return size;
}
