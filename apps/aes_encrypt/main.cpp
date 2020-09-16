#include <iostream>
#include<unistd.h>
#include <fc/crypto/aes.hpp>

using namespace std;




// std::string read_pass() {
//     char pass[256];
//     int i=0;
// 	while((pass[i]=getch())!=13)
// 	{
// 		cout<<"*";
// 		i++;
// 	}
//     return string(pass, pass+i);
// }

int main() {
    try{
        string plan_text;
        cout<<"input to encrypt text:"<<endl;
        getline(cin,plan_text);
        cout<<"to encrypt text:"<<plan_text<<endl;
        string pass = getpass("input password:");
        if (pass!=getpass("confirm password:")) {
            cerr<<"password fonfirm is wrong!";
            return 0;
        }
        std::string encrypted_str = fc::crypto::cfb_aes_encrypt(pass, plan_text);
        if(plan_text!=fc::crypto::cfb_aes_decrypt(pass, encrypted_str)) {
            cerr<<"use pass cfb_aes_decrypt to error!";
            return 0;
        }
        cout<<"encrypt success:"<<encrypted_str<<endl;
    } catch(...) {
        cerr<<"throw error!";
    }
    return 1;
}