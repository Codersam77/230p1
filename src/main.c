#include <data.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  DataNode b0 = {'5', NULL};
  DataNode b1 = {'0', &b0};
  DataNode b2 = {'2', &b1};

  Data d205;
  d205.base = 10;
  d205.sign = 0;
  d205.len = 3;
  d205.number_bits = 8;
  d205.data = &b2;

  Data b11001101 = convert_to_base_n(d205, 2);
  printf("The base should be 2, and your base is %d\n", b11001101.base);
  printf("The sign should not be changed, and your sign is %d\n",
         b11001101.sign);
  printf("The number_bits should not be changed, and your number_bits is %d\n",
         b11001101.number_bits);
  printf("The length should be 8, and your len is %d\n", b11001101.len);
  printf("The data should be 11001101, and your data is ");
  for (DataNode *node = b11001101.data; node; node = node->next) {
    printf("%c", node->number);
  }
  printf("\n\n");

  Data hCD = convert_to_base_n(d205, 16);
  printf("The base should be 16, and your base is %d\n", hCD.base);
  printf("The sign should not be changed, and your sign is %d\n", hCD.sign);
  printf("The number_bits should not be changed, and your number_bits is %d\n",
         hCD.number_bits);
  printf("The length should be 2, and your len is %d\n", hCD.len);
  printf("The data should be CD, and your data is ");
  for (DataNode *node = hCD.data; node; node = node->next) {
    printf("%c", node->number);
  }
  printf("\n\n");

  Data b00110100 = left_shift(d205, 2);
  printf("The base should be 2, and your base is %d\n", b00110100.base);
  printf("The sign should not be changed, and your sign is %d\n",
         b00110100.sign);
  printf("The number_bits should not be changed, and your number_bits is %d\n",
         b00110100.number_bits);
  printf(
      "The length should be 6 since it cannot exceed the number_bit and"
      " the first two 0 should not be included in the list.Your len is %d\n",
      b00110100.len);
  printf("The data should be 110100, and your data is ");
  for (DataNode *node = b00110100.data; node; node = node->next) {
    printf("%c", node->number);
  }
  printf("\n");

  Data d254 = convert_int_to_data(-2, 10, 8);
  printf("The base should be 10, and your base is %d\n", d254.base);
  printf("The sign should be 1, and your sign is %d\n", d254.sign);
  printf("The number_bits should be 8, and your number_bits is %d\n",
         d254.number_bits);
  printf("The length should be 3. Your len is %d\n", d254.len);
  printf("The data should be 254, and your data is ");
  for (DataNode *node = d254.data; node; node = node->next) {
    printf("%c", node->number);
  }
  printf("\n");

  Data b1110 = convert_int_to_data(-2, 2, 4);
  printf("The base should be 2, and your base is %d\n", b1110.base);
  printf("The sign should be 1, and your sign is %d\n", b1110.sign);
  printf("The number_bits should be 4, and your number_bits is %d\n",
         b1110.number_bits);
  printf(
      "The length should be 4 since the original data is truncated. Your len "
      "is %d\n",
      b1110.len);
  printf("The data should be 1110, and your data is ");
  for (DataNode *node = b1110.data; node; node = node->next) {
    printf("%c", node->number);
  }
  printf("\n");

  return 0;
}
