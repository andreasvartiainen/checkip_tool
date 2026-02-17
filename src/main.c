#include <assert.h>
#include <bits/getopt_core.h>
#include <stdint.h>
#include <stdio.h>
#include "helpers.h"
#include <getopt.h>

#define IPV4_BITS 32

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

u32 bin_to_dec(){
  return 0;
}

typedef struct ipv4_t {
  u32 oct_0;
  u32 oct_1;
  u32 oct_2;
  u32 oct_3;
} ipv4_t;

u32 ipv4_to_dec(const ipv4_t ip_addr) {
  u32 value = 0;

  // bit shift and or to get the numbers
  value |= ip_addr.oct_0;
  value <<= 8;
  value |= ip_addr.oct_1;
  value <<= 8;
  value |= ip_addr.oct_2;
  value <<= 8;
  value |= ip_addr.oct_3;

  return value;
}

bool is_valid_ipv4(const ipv4_t ip_addr) {
  if (ip_addr.oct_0 < 0 || ip_addr.oct_0 > 255) {
    return false;
  }
  if (ip_addr.oct_1 < 0 || ip_addr.oct_1 > 255) {
    return false;
  }
  if (ip_addr.oct_2 < 0 || ip_addr.oct_2 > 255) {
    return false;
  }
  if (ip_addr.oct_3 < 0 || ip_addr.oct_3 > 255) {
    return false;
  }
  return true;
}

ipv4_t get_ip_from_string(const char *string) {
  ipv4_t ip_addr = {0};

  assert(sscanf(string, "%u.%u.%u.%u",
      &ip_addr.oct_0,
      &ip_addr.oct_1,
      &ip_addr.oct_2,
      &ip_addr.oct_3
  ) == 4);

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
  printf("%u.%u.%u.%u\n",
      ip_addr.oct_0,
      ip_addr.oct_1,
      ip_addr.oct_2,
      ip_addr.oct_3
  );
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

#define INTERACTIVE 1
#define COMPARE 2
u32 flags = {0};

int main(int argc, char **argv) {
  // get options from commandline with getops library
  i8 opt = {0};
  while ((opt = (i8)getopt(argc, argv, "i")) > 0) {
    switch (opt) {
    case 'i':
      flags |= INTERACTIVE;
      break;
    default:
    }
  }

  ipv4_t ip_addr = {0};
  ipv4_t ip_addr2 = {0};
  ipv4_t subnet_mask =  {0};

  if (flags & INTERACTIVE) {
    ip_addr = get_ip_from_input("Give an IP address: ");
    subnet_mask = get_ip_from_input("Give a Subnet Mask: ");
  } else if (argc == 3){
    ip_addr = get_ip_from_string(argv[1]);
    subnet_mask = get_ip_from_string(argv[2]);
  } else if (argc == 4){
    flags |= COMPARE;
    ip_addr = get_ip_from_string(argv[1]);
    ip_addr2 = get_ip_from_string(argv[2]);
    subnet_mask = get_ip_from_string(argv[3]);
  } else {
    printf("no input provided use -i flag to enter interactive mode\n");
    return 0;
  }

  // convert the ip addresses to u32
  u32 ipv4_dec = ipv4_to_dec(ip_addr);
  u32 ipv4_2_dec = ipv4_to_dec(ip_addr2);
  u32 subnet_dec = ipv4_to_dec(subnet_mask);

  if (flags & COMPARE) {
    // print comparation of 2 ip addresses and a subnet mask
    u32 masked_address_1 = ipv4_dec & subnet_dec;
    u32 masked_address_2 = ipv4_2_dec & subnet_dec;
    if (masked_address_1 == masked_address_2) {
      printf("the addresses are in the same subnet\n");
    } else {
      printf("the belong to different subnets\n");
    }
    print_ipv4(ip_addr);
    print_ipv4_binary(ip_addr);
    print_ipv4(ip_addr2);
    print_ipv4_binary(ip_addr2);
    print_ipv4(subnet_mask);
    print_ipv4_binary(subnet_mask);
  }else {
    // print the information about ip address and subnet
    print_ipv4(ip_addr);
    print_ipv4_binary(ip_addr);
    print_ipv4(subnet_mask);
    print_ipv4_binary(subnet_mask);
    printf("/%u\n", count_ones_in_bin(subnet_dec));
    printf("available hosts: %u\n", ~subnet_dec + 1);

    u32 masked_ipv4 = ipv4_dec & subnet_dec;
    printf("ipv4 masked with subnet-mask (&)\n");
    dec_to_bin(masked_ipv4, IPV4_BITS);
    printf("\n");
  }
	return 0;
}
