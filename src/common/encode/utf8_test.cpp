#include <iostream>   
#include "utf8.h"   
#include "string.h"   
#include "sellib.h"   
int main(int argc,char *argv[]){   
    std::string text = utf8("赵纯华");   
    dnc::utf8_const_iterator it(text.data()+5);   
   
    std::cout<<dnc::string_cast<char>(it)<<std::endl;   
    return 0;   
}   