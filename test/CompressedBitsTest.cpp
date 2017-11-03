#include "test_object.h"
#include "CompressedBits.h"

bool compressed_bits_test(){
    CompressedBits bits1;
    for(int i = 0;i < 1000; i+= 133){
        bits1.add(i);
    }
    CompressedBits bits2;
    for(int j = 0; j < 12123; j += 3){
        bits2.add(j);
    }
    CompressedBits andval = bits1;
    andval &=  bits2;
    CompressedBits orval = bits1;
    orval |=  bits2;
    t_assert(bits1.count() == 8);
    t_assert(bits1.any());
    t_assert(bits2.count() == 12123/3);
    t_assert(andval.count() == 3);
    t_assert(orval.count() == bits1.count() + bits2.count() - andval.count());
    return true;
}
