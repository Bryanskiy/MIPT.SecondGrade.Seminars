#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum flags {
    OK,
    BAD_ALLOC,
    WRONG_INPUT,
};

// если можно избежать использование глобальных переменных, то так лучше и сделать
// лучше, если ф-и будут явно возвращать код возврата
int _FLAG_ = OK;

enum consts {
    ALLOC_START = 5,
    ALLOC_EXPAND = 2,
};

struct number {
    unsigned char* digits;
    unsigned base;
    unsigned len;
};

unsigned char char_to_int(char c) {
    if ( c >= '0' && c <= '9'){
        return c - '0';
    }

    else if ( c >= 'A' && c <= 'Z'){
        return c - 'A' + 10;
    }

    else if ( c >= 'a' && c <= 'z'){
        return c - 'a' + 10;
    }
    // есть ветка условия, которая ничего не возращает ...
    // в данном случае лучше использовать готовую ф-ю, например, atoi
}

struct number read_number(FILE* in) {
    struct number ret = {NULL, 0, 0};

    ret.digits = (char*)calloc(ALLOC_START, sizeof(unsigned char));
    if(ret.digits == NULL) {
        _FLAG_ = BAD_ALLOC;
        return ret;
    }

    unsigned cap = ALLOC_START;

    char digit;
    while(fscanf(in ,"%c", &digit) == 1) {
        if(isspace(digit)) {
            break;
        }

        if(!isalnum(digit)) {
            _FLAG_ = WRONG_INPUT;
        }

        if(ret.len == cap) {
            cap *= ALLOC_EXPAND;
            ret.digits = (char*)realloc(ret.digits, cap);
            if(ret.digits == NULL) {
                _FLAG_ = BAD_ALLOC;
                return ret;
            }
        }

        ret.digits[ret.len++] = char_to_int(digit);
    }

    scanf("%u", &ret.base);
    return ret;
}

unsigned char int_to_char(unsigned char c) {
    if(c >= 0 && c <= 9 ){
        return c + '0';
    }

    else {
        return c - 10 + 'A';
    }
}


void print_number(struct number num) {
    for(unsigned i = 0; i < num.len; ++i) {
        printf("%c", int_to_char(num.digits[i]));
    }
}

int is_zero(struct number number) {
    for(int i = 0; i < number.len; ++i) {
        if(number.digits[i] != 0) {
            return 0;
        }
    }

    return 1;
}

// ожидаемые названия аргументов ф-и divide - это "делимое" и "делитель" ... или "число" и "делитель"
// добавьте, пожалуйста, комментарий, что возвращаемое значение - это остаток от деления
char divide(struct number number, unsigned new_base) {
    int temp = 0;
	for (unsigned i = 0; i < number.len; i++){
		temp = temp * number.base + number.digits[i];
		number.digits[i] = temp / new_base;
		temp = temp % new_base;
	}

	return temp;
}

void reverse(struct number number) {
    for(unsigned i = 0; i < number.len / 2; ++i) {
        unsigned char tmp = number.digits[i];
        number.digits[i] = number.digits[number.len - i - 1];
        number.digits[number.len - i - 1] = tmp;
    }
}

struct number change_base(struct number number, unsigned new_base) {
    struct number ret = {NULL, 0, 0};

    ret.digits = (char*)calloc(ALLOC_START, sizeof(unsigned char));
    if(ret.digits == NULL) {
        _FLAG_ = BAD_ALLOC;
        return ret;
    }

    unsigned cap = ALLOC_START;

    while(!is_zero(number)) {
        if(ret.len == cap) {
            cap *= ALLOC_EXPAND;
            ret.digits = (char*)realloc(ret.digits, cap);
            if(ret.digits == NULL) {
                _FLAG_ = BAD_ALLOC;
                return ret;
            }
        }

        char digit = divide(number, new_base);
        ret.digits[ret.len++] = digit;
    }

    reverse(ret);
    ret.base = new_base;
    return ret;
}

void delete_number(struct number number) {
    if(number.digits != NULL) {
        free(number.digits);
	// лучше занулить указатель после free. чтобы по ошибке не начать использовать указатель после очищения памяти
    }
}

int main() {
    // хоть вы и завели переменную для кода ошибок, но никак её не используете
    struct number input_num = read_number(stdin);

    unsigned new_base;
    scanf("%u", &new_base);

    input_num = change_base(input_num, new_base);
    print_number(input_num);

    delete_number(input_num);
}
