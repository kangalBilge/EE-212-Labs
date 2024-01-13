/*

Author: Nobody
*/

#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
//This is how you import a header file
#include "utils.h"
int gcd(int a, int b);
int mod_inverse(int a, int m);
int generate_keypair(int n, int m, int choice);
int power(int base, int exponent, int p);
//This is where the code runs. Programs start from here
int main(void){
	//Initialize LCD and keypad
	LCD_init();
	keypad_init();
	int p = 59;
	int q = 61;
	int n = p * q;
	int m = (p-1) * (q - 1);
	int option;
	int pass = 0;
	while (pass == 0) {
		print_fnc("A=Enc. D=Dec.:");
		option = get_number_from_keypad();
		clear_lcd();
		if ((option == 17) | (option == 20)){
			clear_lcd();
			pass = 1;
		}
		else {
			print_fnc("Incorrect choice");
			Delay(1000000);
			clear_lcd();
			}
		}
	int block_1, block_2;
	print_fnc("Enter Password:");
	LCD_command(0xC0);
	long key = get_number_from_keypad();
	block_1 = key / 10000;
	block_2 = key % 10000;
	int code = generate_keypair(n,m,option);
	int block_1_rew = power(block_1, code, n );
	int block_2_rew = power(block_2, code, n );
	long message = block_1_rew * 10000 + block_2_rew;
	char message_string[8];
	sprintf(message_string,"%ld",message);
	clear_lcd();
	if (option == 17){
	print_fnc("Encryp Message:");
	}
	else {
	print_fnc("Decryp Message:");
	}
	LCD_command(0xC0);		
	print_fnc(message_string);
	blink();

}




int gcd(int a, int b) {

    while (b) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int a, int m) {

    int m0 = m, x0 = 0, x1 = 1;
    while (a > 1) {
        int q = a / m;
        int temp = m;
        m = a % m;
        a = temp;
        temp = x0;
        x0 = x1 - q * x0;
        x1 = temp;
    }
    return (x1 < 0) ? x1 + m0 : x1;
}

int generate_keypair( int n, int m, int choice) {

    int e = 2;
    while (gcd(m, e) != 1) {
        e += 1;
    }

    int d = mod_inverse(e, m);

		return (choice != 20) ? e: d;
		
}
int power(int base, int exponent, int p)
{
    int res = 1;
 
    while (exponent > 0) {
 
        if (exponent % 2 == 1){
            res = (res * base);
						res = res % p;
					
				}

        exponent = exponent / 2;
        base = (base * base);
				base = base % p;
    }
    return (res < 0) ? res + p: res;
	}

