#include<iostream>
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<windows.h>
using namespace std;
char m[600000];
string bcode="",b64c="";
const string dict="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
int CN2Utf8(char input[],char output[]){
	int len;
	//printf("%d\n",strlen(input));
	wchar_t *out=(wchar_t *)malloc(strlen(input)*sizeof(wchar_t));
	len=MultiByteToWideChar(CP_ACP,0,input,-1,out,0);
	MultiByteToWideChar(CP_ACP,0,input,-1,out,len);
	len=WideCharToMultiByte(CP_UTF8,0,out,wcslen(out),output,0,NULL,NULL);
	WideCharToMultiByte(CP_UTF8,0,out,wcslen(out),output,len,NULL,NULL);
	/*for(int i=0;i<len;i++){
		printf("%d ",output[i]);
	}
	printf("%s",output);
	printf("\n");*/
	//free(out);
	return 1;
}
int Utf82CN(char input[],char output[]){
	int len;
	//printf("%d\n",strlen(input));
	wchar_t *out=(wchar_t *)malloc(strlen(input)*sizeof(wchar_t));
	len=MultiByteToWideChar(CP_UTF8,0,input,-1,out,0);
	MultiByteToWideChar(CP_UTF8,0,input,-1,out,len);
	len=WideCharToMultiByte(CP_ACP,0,out,wcslen(out),output,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,out,wcslen(out),output,len,NULL,NULL);
	output[len]='\0';
	/*for(int i=0;i<len;i++){
		printf("%d ",output[i]);
	}*/
	//printf("%s",output);
	//printf("\n");
	//free(out);
	return 1;
}
string bt(char c){											//get a byte
	int vl=c;
	string en;
	char b[9]="";
	if(vl<0)vl+=256;
	for(int i=1;i<=8;i++){
		b[8-i]=vl%2+48;
		vl/=2;
	}
	en=b;
	return en;
}
string dig6(int v){											//get a 6-bit
	string en;
	char b[7]="";
	for(int i=1;i<=6;i++){
		b[6-i]=v%2+48;
		v/=2;
	}
	en=b;
	return en;
}
string encd(char a[]){										//encoding
	CN2Utf8(a,a);
	//printf("%d ",strlen(a));
	string byte;
	int lc,imp=0,bv=0,l=strlen(a);
	for(int i=0;i<l;i++){
		byte=bt(a[i]);
		bcode+=byte;
	}
	lc=bcode.length();
	if(lc%6==2){
		imp=2;
		bcode+=string("0000");
	}else if(lc%6==4){
		imp=1;
		bcode+=string("00");
	}else if(lc%6==0){
	}else{
		return "";
	}
	for(int i=0;i<lc;i+=6){
		bv=0;
		for(int j=0;j<=5;j++){
			bv*=2;
			bv+=bcode[i+j]-48;
		}
		b64c+=dict[bv];
	}
	for(int i=1;i<=imp;i++){
		b64c+='=';
	}
	return b64c;
}
string decd(string a){
	int l=a.length(),v=0,flag=0,lc;
	//cout<<int(a[l-1])<<endl;
	if (l%4)return "";
	string subc="",bt3="",fin="";
	char dr[600000]="";
	for(int i=0;i<l;i+=4){
		bt3="";
		for(int j=0;j<4;j++){
			if(dict.find(a[i+j])!=dict.npos){
				v=dict.find(a[i+j]);
				if(v==64)flag++;
				else bt3+=dig6(v);
			}else{
				return "";
			}
		}
		subc+=bt3;
	}
	lc=subc.length()-flag*2;
	if(lc%8)return "";
	else{
		for(int i=0;i<lc;i+=8){
			v=0;
			for(int j=0;j<8;j++){
				v*=2;
				v+=subc[i+j]-48;
			}
			if(v>127)v-=256;
			dr[i/8]=char(v);
		}
		//cout<<strlen(dr)<<endl;
		Utf82CN(dr,dr);
		fin=dr;
		return fin;
	}
}
int main(){
	int i=0,flag;
	string s,base64;
	printf("=====BASE64 Encoder/Decoder by AXIS5=====\nSelect Mode(encrypt=1,decrypt=2):");
	cin>>flag;
	cin.ignore();
	if(flag==1){
		printf("Please input the text that you wanted to encrypt(Enter then Ctrl+Z and Enter to stop):\n");
		while(!cin.eof()){
			cin.get(m[i]);
			i++;
		}
		if(strlen(m))m[strlen(m)-1]=0;
		s=encd(m);
	}else{
		printf("Please input the Base64 code that you wanted to decrypt(New line will be ignored,Enter then Ctrl+Z and Enter to stop):\n");
		while(!cin.eof()){
			cin.get(m[i]);
			if(m[i]!='\n')i++;																	//cannot read '\n' when pressed CTRL+Z
		}
		if(strlen(m))m[strlen(m)-1]=0;
		base64=m;
		//cout<<base64<<endl;
		s=decd(base64);
	}
	if(s=="")cout<<"error!"<<endl;
	else cout<<s<<endl;
	system("pause");
	return 0;
}