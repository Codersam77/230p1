#include <data.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* I will create a helper method with a new DataNode. */
static DataNode* create_node(unsigned char digit){
    DataNode* node = (DataNode*)malloc(sizeof(DataNode));
    node->number = digit;//e.g. 0-9 or A-F
    node->next = NULL;
    return node;
}

/*Helper method to append a node to the end of the linked list and return the head*/
static DataNode* append_node(DataNode* head, unsigned char digit){
    DataNode* newN = create_node(digit);
    if (!head){
        return newN;
     }
     DataNode* cur = head;
    while (cur->next){
        cur = cur-> next;
    }
    cur->next = newN;
    return head;
}

/*Helper method to Prepend the node o the front of the list and return the new head*/
static DataNode* prepend_node(DataNode* head, unsigned char digit){
    DataNode* newN = create_node(digit);
    newN->next = head;
    return newN;
 }

static int data_to_int(const Data *src) {
    // We will accumulate in an unsigned 32-bit, then sign-extend if needed.
    unsigned long long acc = 0;  // use 64-bit to avoid overflow
    DataNode *cur = src->data;

    // Read each digit in base src->base
    while (cur) {
        int digitVal = convertCharToNumber(cur->number);
        acc = acc * src->base + (unsigned long long)digitVal;

        // Only keep src->number_bits bits
        if (src->number_bits < 32) {
            unsigned long long mask = ((unsigned long long)1 << src->number_bits) - 1;
            acc &= mask;
        }
        cur = cur->next;
    }

    if (src->sign == 1 && src->number_bits <= 32) {
        unsigned long long signBit = (unsigned long long)1 << (src->number_bits - 1);
        if (acc & signBit) {
            unsigned long long mask = ~(((unsigned long long)1 << src->number_bits) - 1);
            acc |= mask;  // fill all high bits with 1
        }
    }

    return (int)acc;
}
static Data build_data_from_int(int value, unsigned char base,
                                unsigned char number_bits, unsigned char sign) {
    Data result;
    result.base = base;
    result.sign = sign;  // typically 1 if we want to allow negatives
    result.number_bits = number_bits;
    result.len = 0;
    result.data = NULL;

    // Special case: if number_bits == 0 or base invalid? (Not expected here, but be safe.)
    if (number_bits == 0 || base < 2 || base > 16) {
        return result;
    }

    // Truncate 'value' to number_bits bits (two's complement if negative).
    // Because 'value' is an int, if it's negative, it already has a top bit set.
    // We'll just mask off anything above number_bits.
    unsigned int mask = 0xFFFFFFFF;
    if (number_bits < 32) {
        mask = (1U << number_bits) - 1;
    }
    unsigned int uval = ((unsigned int)value) & mask;

    // If everything is zero:
    if (uval == 0) {
        // We want a single node '0'
        result.data = create_node('0');
        result.len = 1;
        return result;
    }

    // Convert to base by repeated mod/div
    // We'll collect digits from LSB to MSB
    while (uval != 0) {
        unsigned int digit = (unsigned int)(uval % base);
        uval /= base;

        // Convert numeric digit to char '0'..'9' or 'A'..'F'
        char ch = convertNumberToChar(digit);

        // We'll prepend so that the final list has MSB at head
        result.data = prepend_node(result.data, ch);
        result.len++;
    }

    // If result is all zeros, ensure we keep exactly one zero:
    if (result.len == 0) {
        // means original value was 0
        result.data = create_node('0');
        result.len = 1;
    }

    return result;
}

int convertCharToNumber(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  } else if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  } else {
    return -1;
  }
}

char convertNumberToChar(int n) {
  if (n >= 0 && n <= 9) {
    return n + '0';
  } else if (n >= 10 && n <= 15) {
    return n - 10 + 'A';
  } else {
    return 0;
  }
}

Data convert_to_base_n(Data src, unsigned char n) {
    int value = data_to_int(&src);

    Data new_data = build_data_from_int(value, n, src.number_bits, src.sign);

    return new_data;
}
Data convert_int_to_data(int number, unsigned char base, unsigned char number_bits) {
    unsigned char sign = 1;

    Data new_data = build_data_from_int(number, base, number_bits, sign);

    return new_data;
}
Data left_shift(Data src, int n) {
    Data new_data;
    int original_value = data_to_int(&src);

    // Perform shift
    int shifted = original_value << n;

    // Mask to number_bits
    if (src.number_bits < 32) {
        unsigned int mask = (1U << src.number_bits) - 1;
        shifted &= mask;
    }

    // Build in base 2
    new_data = build_data_from_int(shifted, 2, src.number_bits, src.sign);
    return new_data;
}

Data right_shift(Data src, int n) {
    Data new_data;
    int original_value = data_to_int(&src);

    // Sign-extend into 64 bits
    long long temp64 = (long long)original_value;
    long long shifted64;

    if (src.sign == 1) {
        // Arithmetic shift
        shifted64 = temp64 >> n;
    } else {
        // Logical shift: cast to unsigned before shifting
        unsigned long long utemp64 = (unsigned long long)temp64;
        shifted64 = (long long)(utemp64 >> n);
    }

    // Mask & optional sign extension for the final number_bits
    if (src.number_bits < 32) {
        unsigned long long mask = ((unsigned long long)1 << src.number_bits) - 1;
        unsigned long long masked = ((unsigned long long)shifted64) & mask;

        if (src.sign == 1 && src.number_bits <= 32) {
            unsigned long long signBit = 1ULL << (src.number_bits - 1);
            if (masked & signBit) {
                unsigned long long fillMask = ~(((unsigned long long)1 << src.number_bits) - 1);
                masked |= fillMask;
            }
        }
        shifted64 = (long long)masked;
    }

    // Build in base 2
    new_data = build_data_from_int((int)shifted64, 2, src.number_bits, src.sign);
    return new_data;
}

