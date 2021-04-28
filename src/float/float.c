#include "float.h"


double __adddf3(double a, double b) {

}


void extract_signbit_as_uint(double source, uint8_t* const destination) {
  uint64_t source_as_uint = BIT_CAST(source, uint64_t);
  *destination = ((uint8_t)(source_as_uint >> 63));
}



void extract_mantissa_as_uint(double source, uint64_t* const destination) {
  uint64_t source_as_uint = BIT_CAST(source, uint64_t);
  set_bits_in_range(52,64,0, &source_as_uint);

/*
  uint64_t result = 0;
  for(uint8_t i = 1; i < 52; ++i) { /// if this doesn't work, try <=
    uint64_t current_bit = ((uint64_t)(get_bit_at_far(52-i,&source_as_uint)));
    result += current_bit * ((uint64_t)(kpow_u(2,52-i)));
  }*/
  while(get_bit_at_far(1,&source_as_uint) != 1) {
    source_as_uint >>=1;
  }
  set_bit_at_far(0,1,&source_as_uint);
  *destination = source_as_uint;
}

void extract_mantissa_as_arbitrarily_large_uint(double source, uint64_t* const destination) {
 //double source_times_arbitrarily_large_power_of_ten = source * 10000000000000000.0;
 //*destination = ((uint64_t)(source_times_arbitrarily_large_power_of_ten));

}

void extract_left_of_radix_as_uint(double source, uint64_t* const destination) {

}

void extract_right_of_radix_as_uint(double source, uint64_t* const destination) {

}

int16_t get_exponent_as_int(double source) { //todo: rename to get_binary_exponent_as_int
  uint64_t source_as_uint = BIT_CAST(source, uint64_t);

  set_bits_in_range(0,52,0,&source_as_uint);
  set_bit_at_far(63,0,&source_as_uint);
  source_as_uint >>= 52;

  int16_t source_as_int = ((int16_t)(source_as_uint)) - 1023; /// correct for bias

  return source_as_int;
}

int16_t get_decimal_exponent_as_int(double source) {
    int16_t binary_exponent = get_exponent_as_int(source);
    return (3*binary_exponent)/10;
}

uint8_t get_signbit(double* source) { /// rename to get_double_signbit
  return get_bit_at_far(63, source);
}

uint64_t get_mantissa_as_uint(double source) {
  int16_t exponent = get_exponent_as_int(source);
  uint8_t signbit = get_signbit(&source);
  //double exponent_cancelled = (source / kpow(10, ((3*exponent)/10))) * ((2*(-1)*signbit)+1); /// try (3* (ksignbit(exponent) == 1 ? kabs(exponent) : 0) + ... whateva figure out later
  double exponent_cancelled = (source * kpow_d(10, ((3*(-exponent))/10))) * ((2*(-1)*signbit)+1); /// try (3* (ksignbit(exponent) == 1 ? kabs(exponent) : 0) + ... whateva figure out later

  /// also maybe just do 3*((-1)*exponent)


  uint64_t big_decimal = (uint64_t)(exponent_cancelled* kpow_u(10, 15));

//  kprintf("%s\n", "big decimal:");
//  kprintf("%u\n", big_decimal);

  uint8_t number_of_trailing_zeros = 0;
  for(uint8_t i = 1; i < 51; ++i) {
    if(kmod((int64_t)big_decimal, kpow(10,i)) != 0) {
      number_of_trailing_zeros = i-1; /// might be off by one
      break;
    }
  }
  uint64_t power_of_10 = kpow_u(10,number_of_trailing_zeros);


    big_decimal /= (power_of_10 == 0 ? 1 : power_of_10);

  return big_decimal;


}

void copy_str_to_position(const char* source, char* destination, uint8_t position, uint8_t source_size) {
  for(uint8_t i = 0; i < source_size; ++i) {
    destination[position + i] = source[i];
  }
}

void double_to_str(double source, char* const destination) {
  int16_t exponent = get_decimal_exponent_as_int(source);
  char mantissa_str[32];
  kuint_to_string(get_mantissa_as_uint(source), mantissa_str, 32);
  //char destination[32];

  uint8_t i = 0;
  if(get_signbit(&source)) {
      destination[0] = '-';
      ++i;
  }
  
  if(exponent < 0) {
    destination[i] = '0';
    destination[i+1] = '.';
    
    i += 2;
    while(i < kabs(exponent)) {
      destination[i] = '0'; /// off by one
      ++i;

    }
    copy_str_to_position(mantissa_str, destination, i, 32);
  }
  else {
    uint8_t i_mantissa = 0;
    for(uint8_t i_ret = 0; i_ret < (uint8_t)kstrlen(mantissa_str) + 1; ++i_ret) {
      destination[i_ret] = mantissa_str[i_mantissa];
      if(i_ret == exponent) {
        destination[i_ret+2] = '.'; /// off by one
        i_ret += 1;
      }
      ++i_mantissa;

    }
  }
  kprintf("%s\n", destination);

  //destination = destination;
  /*
  while(i < exponent) {
    destination[i] = temp_buf[i];
    ++i;
  }
  destination[i] = '.';
  while(i < )
   */
  
}

/*
void extract_mantissa_as_uint(double source, uint64_t* const destination) {
  uint64_t source_as_uint = BIT_CAST(source, uint64_t);
  set_bits_in_range(52,64,0, &source_as_uint);
  while(get_bit_at_far(0,&source_as_uint) != 1) {
    source_as_uint >>=1;
  }
  for(uint8_t i = 52; i >= 0; --i) {
    if(get_bit_at_far(i,&source_as_uint)) {
      set_bit_at_far(i+1,1,&source_as_uint);
      break;
    }
  }
  *destination = source_as_uint;
}*/


//void extract_mantissa(double source, )


