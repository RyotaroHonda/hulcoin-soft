#include"DwgFuncs.hh"

#include<stdint.h>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<bitset>
#include<tuple>

// DWG --------------------------------------------------------------
uint64_t
SetDWG(uint32_t local_address, uint32_t delay, uint32_t width)
{
  const uint32_t kMaxDelay = 31;
  const uint32_t kMaxWidth = 32;
  
  if(delay > kMaxDelay){
    std::cerr << "#E: DWG delay requirement is exceeded 31."
	      << "    Address: 0x" << std::hex << local_address << std::dec
	      << "    Delay:     " << delay
	      << std::endl;
    std::cerr << "    Delay value of 31 is forced to set"
	      << std::endl;

    delay = kMaxDelay;
  }

  if(width > kMaxWidth){
    std::cerr << "#E: DWG width requirement is exceeded 32."
	      << "    Address: 0x" << std::hex << local_address << std::dec
	      << "    Width:     " << width
	      << std::endl;
    std::cerr << "    Width value of 32 is forced to set"
	      << std::endl;

    width = kMaxWidth;
  }

  if(width == 0){
    std::cerr << "#E: DWG width 0 is now allowed."
	      << "    Address: 0x" << std::hex << local_address << std::dec
	      << "    Width:     " << width
	      << std::endl;
    std::cerr << "    Width value of 1 is forced to set"
	      << std::endl;

    width = 1;
  }
  
  // Make 64-bit vector
  const uint32_t kLengthSR = 64;

  uint32_t delay_count = delay;
  uint32_t width_count = width;
  
  std::bitset<kLengthSR> bits;
  bits.reset();
  for(int32_t i = kLengthSR-1; i>=0; --i){
    if(delay_count != 0){
      --delay_count;
      continue;
    }

    if(width_count != 0){
      bits.set(i);
      --width_count;
    }
  }

  uint64_t reg64 = bits.to_ullong();
  return reg64;
}
