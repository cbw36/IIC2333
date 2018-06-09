#include <stdio.h>
#include <math.h>


int hex_binary(char hex[], int binary_number[]);
int * dec_to_bin(int dec_name, int*bin_name, int ind);




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

//int main()
//{
//  char hex[20];
//  int binary_number[100];
//  int j,k;
//
//  printf("Enter hexadecimal number: ");
//  scanf("%19s",hex);
//  j = hex_binary(hex,binary_number);
//
//  printf("Binary number is: ");
//  for(k=j-1;k>=0;k--)
//  printf("next %d",binary_number[k]);
//  printf("\n");
//
//  return 0;
//}