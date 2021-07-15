#include "Parser.h"
#include "LAN.h"
//buf_size

bool parser::compStr(char *Str1, const char *Str2){
  for(int i=0;i<buf_size;i++){
    if(pgm_read_byte(Str2) == 0){
      return true;
    }
    else{
      if(*Str1 != pgm_read_byte(Str2)){
          return false;
      }
    }
    Str1++;
    Str2++;
  }
}

void parser::toUpCase(char *Buf){
  for (int i = 0; i < buf_size; i++) {
    if (*Buf == 0) {
      return;
    }
    else {
      if ((*Buf >= 97) && (*Buf <= 122)) {
        *Buf -= 32;
      }
    }
    Buf++;
  }
}

bool parser::parStr(char *Buf, unsigned char *Rez){
  unsigned char numb;
  *Rez = 0;
  for (int i = 0; i < 3; i++) {
    numb = *Buf - 48;
    if (numb > 9) {
      if(i == 0)  {return false;}
      else if(*Buf == 0) {return true;}
      else if(*Buf == '.') {return true;}
      else        {return false;}
    }
    else {
      *Rez = (*Rez <<3) + (*Rez << 1) + numb;
    }
    Buf++;
  }
  return true;
}

bool parser::parseMAC(char *Buf, unsigned char *MAC){
  for(char i = 0; i < 6; i++){
    *MAC = 0;
    for(char j = 0; j < 2; j++){
      switch(*Buf){
        case '0':
          *MAC = (*MAC << 4) + 0;
          break;
        case '1':
          *MAC = (*MAC << 4) + 1;
          break;
        case '2':
          *MAC = (*MAC << 4) + 2;
          break;
        case '3':
          *MAC = (*MAC << 4) + 3;
          break;
        case '4':
          *MAC = (*MAC << 4) + 4;
          break;
        case '5':
          *MAC = (*MAC << 4) + 5;
          break;
        case '6':
          *MAC = (*MAC << 4) + 6;
          break;
        case '7':
          *MAC = (*MAC << 4) + 7;
          break;
        case '8':
          *MAC = (*MAC << 4) + 8;
          break;
        case '9':
          *MAC = (*MAC << 4) + 9;
          break;
        case 'A':
          *MAC = (*MAC << 4) + 10;
          break;
        case 'B':
          *MAC = (*MAC << 4) + 11;
          break;
        case 'C':
          *MAC = (*MAC << 4) + 12;
          break;
        case 'D':
          *MAC = (*MAC << 4) + 13;
          break;
        case 'E':
          *MAC = (*MAC << 4) + 14;
          break;
        case 'F':
          *MAC = (*MAC << 4) + 15;
          break;
        default:
          return false;
          break;
      }
      Buf++;
    }
    
    if(*Buf == ':') {
      Buf++; 
      MAC++;
    }
    else if(i == 5) {
      return true;
    }
    else {
      return false;
    }
  }
}

bool parser::parseIP(char *Buf, unsigned char *IP){
  for(char i = 0; i < 4; i++){
    if(parStr(Buf, IP)){
      if(*IP < 10)      { Buf = Buf + 2; }
      else if(*IP < 100){ Buf = Buf + 3; }
      else              { Buf = Buf + 4; }
    }
    else{
      return false;
    }
    IP++;
  }
  return true;
}

bool parser::parsePM(char *Buf, float *Coef){
  float frac_mult;
  bool int_part;
  bool neg_value;
  unsigned char numb;

  for(char i = 0; i < 2; i++){
    *Coef = 0;  
    int_part = true;
    neg_value = false;
    frac_mult = 0.1;
    for (char j = 0; j < 16; j++){
      numb = *Buf - 48;
      if(numb > 9){
        if(j == 0){
          if(*Buf == '-') { neg_value = true; }
          else { return false; }
        }
        else if(*Buf == 0) { break; }
        else if(*Buf == '.') { int_part = false; }
        else if(*Buf == ',') { Buf++; break; }
        else { return false; }
      }
      else{
        if(int_part){
          *Coef = (*Coef * 10) + numb;
        }
        else{
          *Coef = *Coef + (numb * frac_mult);
          frac_mult /= 10;
        }
      }
      Buf++;
    }
    if(neg_value) {*Coef *= -1.0;}
    Coef++;
  }
  return true;
}

void parser::bufNull(char *Buf, unsigned char Length){
  for(unsigned char i = 0; i < Length; i++){
    *Buf = 0; 
    Buf++;
  }
}
