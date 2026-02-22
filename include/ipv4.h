#ifndef STUFF_H
#define STUFF_H

#include "helpers.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define IPV4_BITS 32
#define BYTE 8 // byte == 8 bits
#define BYTE_MASK 0xFF // mask where 1111 1111
// flags for different modes 
#define INTERACTIVE 1
#define COMPARE 2
#define WILDCARD 4
#define SLASH 8
u32 flags = {0};


typedef struct ipv4_t {
  u32 oct_0;
  u32 oct_1;
  u32 oct_2;
  u32 oct_3;
} ipv4_t;

// reverse a string
void reverse_string(char **str) {
  u64 len = strlen(*str);

  // allocate size of the old string + one for nulll
  char *tmp = malloc(sizeof(char) * (len + 1));

  for (u64 i = 1; i <= len; i++) {
    tmp[len - i] = (*str)[i - 1];
  }

  free(*str);

  tmp[len] = '\0';

  *str = tmp;
}

// convert a number into a string
char *num_to_string(u32 num) {
  char *str = (char*)malloc(sizeof(char) * 32);

  int index = 0;
  if (num == 0) {
    str[index] = '0';
    str[index + 1] = '\0';
    return str;
  }

  while (num > 0 && index < 32) {
    str[index] = (i8)(num % 10) + '0';
    num /= 10;
    index++;
  }

  str[index] = '\0';
  reverse_string(&str);
  return str;
}

// convert ipv4 address to a string
char *ipv4_to_string(const ipv4_t ip_0) {
  char *str = (char*)malloc(sizeof(char) * 128);

  strcat(str, num_to_string(ip_0.oct_0));
  strcat(str, ".");
  strcat(str, num_to_string(ip_0.oct_1));
  strcat(str, ".");
  strcat(str, num_to_string(ip_0.oct_2));
  strcat(str, ".");
  strcat(str, num_to_string(ip_0.oct_3));

  return str;
}

// print decimal as binary recursively (to make it right way around)
void dec_to_bin(u32 num, u8 bits) {
  if (bits == 0) {
    return;
  }
  dec_to_bin(num >> 1, --bits);
  if (bits % 4 == 0 && bits != 0) {
    printf(" ");
  }
  printf("%u", num & 0x1);
}

u32 bin_to_dec() { return 0; }

u32 ipv4_to_dec(const ipv4_t ip_addr) {
  u32 value = 0;

  // bit shift and or to get the numbers
  value |= ip_addr.oct_0;
  value <<= BYTE;
  value |= ip_addr.oct_1;
  value <<= BYTE;
  value |= ip_addr.oct_2;
  value <<= BYTE;
  value |= ip_addr.oct_3;

  return value;
}

ipv4_t dec_to_ipv4(u32 num) {
  ipv4_t ip_addr = {0};

  ip_addr.oct_3 = num & BYTE_MASK;
  num >>= BYTE;
  ip_addr.oct_2 = num & BYTE_MASK;
  num >>= BYTE;
  ip_addr.oct_1 = num & BYTE_MASK;
  num >>= BYTE;
  ip_addr.oct_0 = num & BYTE_MASK;

  return ip_addr;
}


bool is_valid_ipv4(const ipv4_t ip_addr) {
  if (ip_addr.oct_0 < 0 || ip_addr.oct_0 > BYTE_MASK) {
    return false;
  }
  if (ip_addr.oct_1 < 0 || ip_addr.oct_1 > BYTE_MASK) {
    return false;
  }
  if (ip_addr.oct_2 < 0 || ip_addr.oct_2 > BYTE_MASK) {
    return false;
  }
  if (ip_addr.oct_3 < 0 || ip_addr.oct_3 > BYTE_MASK) {
    return false;
  }
  return true;
}

ipv4_t subnet_from_slash_format(const char *string) {
  u32 ones = 0;
  u32 subnet = (u32)(-1);

  assert(sscanf(string, "/%u", &ones) == 1);

  ones = IPV4_BITS - ones;

  // bit shift
  subnet <<= ones;

  return dec_to_ipv4(subnet);
}

ipv4_t get_ip_from_string(const char *string) {
  ipv4_t ip_addr = {0};

  assert(sscanf(string, "%u.%u.%u.%u", &ip_addr.oct_0, &ip_addr.oct_1,
                &ip_addr.oct_2, &ip_addr.oct_3) == 4);

  assert(is_valid_ipv4(ip_addr));

  return ip_addr;
}

ipv4_t get_ip_from_input(const char *prompt) {
  printf("%s", prompt);
  char *line defer = nullptr;
  size_t size = 0;
  getline(&line, &size, stdin);
  strip_newline(line);

  ipv4_t ip_addr = get_ip_from_string(line);

  return ip_addr;
}

void print_ipv4(const ipv4_t ip_addr) {
  printf("%u.%u.%u.%u\n", ip_addr.oct_0, ip_addr.oct_1, ip_addr.oct_2,
         ip_addr.oct_3);
}

void print_ipv4_binary(const ipv4_t ip_addr) {
  u32 ipv4_dec = ipv4_to_dec(ip_addr); // ipv4 in decimal

  // print the address in binary form
  dec_to_bin(ipv4_dec, IPV4_BITS);
  printf("\n");
}

u32 count_ones_in_bin(u32 num) {
  u32 count = 0;
  while (num > 0) {
    if ((num & 1)) {
      count++;
    }
    num >>= 1;
  }
  return count;
}

void ipv4_print_compare_with_mask(ipv4_t ip_0, ipv4_t ip_1, ipv4_t mask) {
    // print comparation of 2 ip addresses and a subnet mask
    u32 ip_0_dec = ipv4_to_dec(ip_0);
    u32 ip_1_dec = ipv4_to_dec(ip_1);
    u32 mask_dec = ipv4_to_dec(mask);

    u32 masked_ipv4 = ip_0_dec & mask_dec;
    u32 masked_ipv4_2 = ip_1_dec & mask_dec;

    if (masked_ipv4 == masked_ipv4_2) {
      printf("the addresses are in the same subnet\n");
    } else {
      printf("the addresses belong to different subnets\n");
    }
    printf("IP-1: %s\n", ipv4_to_string(ip_0));
    print_ipv4_binary(ip_0);
    printf("IP-2: %s\n", ipv4_to_string(ip_1));
    print_ipv4_binary(ip_1);
    printf("SUBNET_MASK: %s\n", ipv4_to_string(mask));
    print_ipv4_binary(mask);
}
void ipv4_info_print_all(ipv4_t ip_0, ipv4_t mask) {
    // print the information about ip address and subnet
    u32 ip_0_dec = ipv4_to_dec(ip_0);
    u32 mask_dec = ipv4_to_dec(mask);

    printf("IP: %s\n", ipv4_to_string(ip_0));
    print_ipv4_binary(ip_0);
    printf("SUBNET_MASK: %s\n", ipv4_to_string(mask));
    print_ipv4_binary(mask);
    printf("/%u\n", count_ones_in_bin(mask_dec));
    printf("available hosts: %u\n", ~mask_dec + 1);
    printf("ipv4 masked with subnet-mask (&)\n");
    dec_to_bin(ip_0_dec, IPV4_BITS);
    printf("\n");
    printf("Network Address\n");
    print_ipv4(dec_to_ipv4(ip_0_dec));
    printf("max address:\n");
    print_ipv4(dec_to_ipv4(ip_0_dec + ~mask_dec));
}

void ipv4_info_print_subnet_mask(ipv4_t mask) {
    u32 mask_dec = ipv4_to_dec(mask);

    ipv4_t wildcard = dec_to_ipv4(~mask_dec);
    printf("SUBNET_MASK: %s\n", ipv4_to_string(mask));
    print_ipv4_binary(mask);
    printf("WILDCARD: %s\n", ipv4_to_string(wildcard));
    print_ipv4_binary(wildcard);
}


#endif
