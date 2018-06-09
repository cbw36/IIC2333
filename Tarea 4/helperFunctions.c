#include <stdio.h>
#include <math.h>


int hex_binary(char hex[], int binary_number[]);
int * dec_to_bin(int dec_name, int*bin_name, int ind);
char* strToBin(char* str);
char* getRivMsg(char* riv_nam, char* resp_id);


int hex_binary(char hex[], int binary_number[])
{
  int i, j=0, length, decimal=0;
  for(length=0; hex[length]!='\0'; ++length);
  for(i=0; hex[i]!='\0'; ++i, --length)
  {
    if(hex[i]>='0' && hex[i]<='9')
      decimal+=(hex[i]-'0')*pow(16,length-1);
    if(hex[i]>='A' && hex[i]<='F')
      decimal+=(hex[i]-55)*pow(16,length-1);
    if(hex[i]>='a' && hex[i]<='f')
      decimal+=(hex[i]-87)*pow(16,length-1);
  }
  //At this point, variable decimal contains the hexadecimal number in decimal format.
  while (decimal!=0)
  {
    binary_number[j++] = decimal%2;
    decimal/=2;
  }
  return j;
}


int * dec_to_bin(int dec_name, int*bin_name, int ind)
{
  if (dec_name == 0){
    return bin_name;
  }
  if (dec_name == 1)
  {
    bin_name[ind] = 1;
    return bin_name;
  }

  else if ((dec_name %2) ==0)
  {
    dec_name = dec_name/2;
    ind --;
    bin_name = dec_to_bin(dec_name, bin_name, ind);
  }
  else
  {
    dec_name = dec_name/2;
    bin_name[ind] = 1;
    ind --;
    bin_name = dec_to_bin(dec_name, bin_name, ind);
  }
  return bin_name;

}

char* strToBin(char* str)
{
  int len = strlen(str);
  char* str_bin = malloc((8 * len)+1);
  int ind = 0;
  int count = 0;

  while (count<len)
  {
    int * char_int = malloc(8*sizeof(int));
    char_int = dec_to_bin(str[count], char_int, 7);

    char* char_bin = malloc(8);
    for (int i = 0; i<8;i++)
      char_bin[i] = char_int[i] + '0';
    for (int i =ind;i<ind+8;i++)
      str_bin[i] = char_bin[i-ind];
    ++count;
    ind = ind+8;
  }
  return str_bin;

}

int* binToInt(char* bin){
  int * msg_vals = malloc(strlen(bin) * sizeof(int)/8);
  for (int i =0; i<strlen(bin);i+=8)
  {
    int b0 = bin[i]-'0'; int b1 = bin[i+1]-'0';int b2 = bin[i+2]-'0';int b3 = bin[i+3]-'0';int b4 = bin[i+4]-'0';int b5 = bin[i+5]-'0';int b6 = bin[i+6]-'0';int b7 = bin[i+7]-'0';
    msg_vals[i/8] = (b0*128) + (b1*64)+ (b2*32)+ (b3*16)+ (b4*8)+ (b5*4)+ (b6*2)+ b7;
  }
  return msg_vals;
}

char* getRivMsg(char* riv_name, char* resp_id)
{

  int len = strlen(riv_name);

  char* name_bin = malloc(8*len);
  name_bin = strToBin(riv_name);

  int* msg_size_int = malloc(8*sizeof(int));
  char* msg_size = malloc(8);
  msg_size_int = dec_to_bin(strlen(riv_name), msg_size_int, 7);
  for (int i = 0;i<8;i++)
    msg_size[i] = msg_size_int[i] + '0';


  char* name_msg = malloc(16 + strlen(name_bin));
  for (int i = 0; i<8;i++)
    name_msg[i] = resp_id[i];
  for (int i = 8; i<16;i++)
    name_msg[i]= msg_size[i-8];
  for (int i = 16;i<16+ strlen(name_bin);i++)
    name_msg[i] = name_bin[i-16];
  puts(name_msg);

  return name_msg;
}


//    printf("%c%c%c%c%c%c%c%c\n",bin[i],bin[i+1],bin[i+2],bin[i+3],bin[i+4],bin[i+5],bin[i+6],bin[i+7]);
