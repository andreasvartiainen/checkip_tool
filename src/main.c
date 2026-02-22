#include "helpers.h"
#include <assert.h>
#include <bits/getopt_core.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include "ipv4.h"

int main(int argc, char **argv) {
  ipv4_t ip_addr = {0};
  ipv4_t ip_addr2 = {0};
  ipv4_t subnet_mask = {0};

  // get options from commandline with getops library
  i8 opt = {0};
  while ((opt = (i8)getopt(argc, argv, "is")) > 0) {
    switch (opt) {
    case 'i':
      flags |= INTERACTIVE;
      break;
    case 's': // slash mode for ip subnet
      flags |= SLASH;
      break;
    default:
    }
  }

  // printf("arguments: %d\n", argc);

  if (flags & INTERACTIVE) {
    ip_addr = get_ip_from_input("Give an IP address: ");
    subnet_mask = get_ip_from_input("Give a Subnet Mask: ");
    ipv4_info_print_all(ip_addr, subnet_mask);
  } else if (flags & SLASH){
    ipv4_t ipv4_subnet = subnet_from_slash_format(argv[2]);
    print_ipv4(ipv4_subnet);
  } else if (argc == 4) {
    ip_addr = get_ip_from_string(argv[1]);
    ip_addr2 = get_ip_from_string(argv[2]);
    subnet_mask = get_ip_from_string(argv[3]);
    ipv4_print_compare_with_mask(ip_addr, ip_addr2, subnet_mask);
  } else if (argc == 3) {
    ip_addr = get_ip_from_string(argv[1]);
    subnet_mask = get_ip_from_string(argv[2]);
    ipv4_info_print_all(ip_addr, subnet_mask);
  } else if (argc == 2) {
    subnet_mask = get_ip_from_string(argv[1]);
    ipv4_info_print_subnet_mask(subnet_mask);
  } else {
    printf("no input provided\nusage:\n \
-i flag to enter interactive mode\n \
<subnet mask> to show the corresponding wildcard\n \
<ip address> <subnet-mask> information about IP address and subnet combination\n \
<ip address> <ip address> <subnet-mask> check if IPs are in the same subnet\n"
   );
    return 0;
  }

  return 0;
}
