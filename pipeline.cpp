//============================================================================
// Name        : Mips64pipelining.cpp
// Author      : Prasanth Vivekanandan
// Version     : 1.0
// Copyright   : EECS.university of kansas
// Description : Pipelining in MIPS 64 Architecture, Ansi-style
//============================================================================
#include <iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include <map>
#include<stdio.h>
#include<sstream>
#include<stdlib.h>
using namespace std;
 map<string,int> regs;
 map<string,float> fpregs;
 map<int, float> mem;
 struct instrnames{
 public:
 string stld[5]={" IF"," ID","EXE","MEM"," WB"};
string addsub[8]={" IF"," ID"," A1"," A2"," A3"," A4","MEM"," WB"};
string mult[11]={" IF"," ID"," M1"," M2"," M3"," M4"," M5"," M6"," M7","MEM"," WB"};
 }a;
 int codelength=0,codewordslength;
 struct code{
 public:
  string instr;
  string reg1;
  string reg2;
  string regi;
  string reg3;
  }c[20];
 struct pipeline
 {
 public:
   string stages[100];

 }pipe[200];
void readfile(char* s)
{
    string word;
    int value;
    float value1;
    float value2;
    ifstream inFile(s);
    if(inFile.fail())
        {cout<<"\n file does not exist";exit(0);}
    else
         {
         inFile >> word;
         if(word == "I-REGISTERS")
          {
            cout << "Reading registers.\n";
            inFile >> word;
            while(word != "FP-REGISTERS"){
            inFile >> value;
            regs[word] = value;
            inFile >> word;
            }
           }
           if(word == "FP-REGISTERS")
          {
            cout << "Reading floating point registers.\n";
            inFile >> word;
            while(word != "MEMORY"){
            inFile >> value1;
            fpregs[word] = value1;
            inFile >> word;
            }
           }
           if(word == "MEMORY")
        {
        cout << "Reading memory.\n";
        inFile >> word;
        while(word != "CODE")
        {
            inFile >> value2;
            mem[atoi(word.c_str())] = value2;
            inFile >> word;
        }
        }
        if(word == "CODE")
        {
        cout<<"Reading code.\n";
        inFile >> word;
        int i;
        for(i=0;!inFile.eof();i++)
         {if((word=="L.D")||(word=="S.D"))
          {c[codelength].instr=word;
          inFile >> word;
          c[codelength].reg1=word;
          inFile >> word;
           c[codelength].regi=word;
          c[codelength].reg2=c[codelength].regi+',';
           c[codelength].reg3="0,";
           codelength++;
          inFile >> word;
          }
          else{ if ((word=="ADD.D")||(word=="SUB.D")||(word=="MUL.D"))
          {
          c[codelength].instr=word;
          inFile >> word;
          c[codelength].reg1=word;
          inFile >> word;
          c[codelength].reg2=word;
          inFile >> word;
          c[codelength].regi=word;
          c[codelength].reg3=c[codelength].regi+',';
          codelength++;
          inFile >> word;
          }
          else
          {cout<<"\nwrong instructions"<<endl; exit(0);}
        }}
        }
         }
 cout<<"\nFinished Reading."<<endl;
 inFile.close();
}
int check_id(int i,int j)
{int c=0;
  for(int k=0;c==0;k++)
  {
    if(pipe[j].stages[k]==" ID")
    {
      return k;
      c=1;
    }

  }
}
void pipelining(int i,int k,int l)
{
if(l==0)
 {if((c[i].instr=="L.D")||(c[i].instr=="S.D"))
                    for(int j=0;j<5;j++)
                    pipe[i].stages[j+k]=a.stld[j];
 if((c[i].instr=="ADD.D")||(c[i].instr=="SUB.D"))
                    for(int j=0;j<8;j++)
                    pipe[i].stages[j+k]=a.addsub[j];
if(c[i].instr=="MUL.D")
                    for(int j=0;j<11;j++)
                    pipe[i].stages[j+k]=a.mult[j];
}
else
   {int z=0,q=0;
   if((c[i].instr=="L.D")||(c[i].instr=="S.D"))
                    for(int j=0;q==0;j++)
                    {
                    if(j==0)
                    {pipe[i].stages[j+k]=a.stld[j];z++;}
                     else
                     {if((j+k)<(l-1))

                     pipe[i].stages[j+k]="  s";
                     else
                     {pipe[i].stages[j+k]=a.stld[z];z++;}
                     if(pipe[i].stages[j+k]==" WB")
                     q=1;
                     }}
    if((c[i].instr=="ADD.D")||(c[i].instr=="SUB.D"))
                    {
                    for(int j=0;q==0;j++)
                    {
                    if(j==0)
                    {
                     pipe[i].stages[j+k]=a.addsub[z];z++;}
                     else{if((j+k)<(l))
                     pipe[i].stages[j+k]="  s";
                     else
                     {pipe[i].stages[j+k]=a.addsub[z];z++;}
                     if(pipe[i].stages[j+k]==" WB")
                     q=1;
                     }}}
     if(c[i].instr=="MUL.D")
                    {
                    for(int j=0;q==0;j++)
                    {
                    if(j==0)
                    {
                     pipe[i].stages[j+k]=a.mult[z];z++;}
                     else{if((j+k)<(l))
                     pipe[i].stages[j+k]="  s";
                     else
                     {pipe[i].stages[j+k]=a.mult[z];z++;}
                     if(pipe[i].stages[j+k]==" WB")
                     q=1;
                     }}}
    }
}
void pipelining1(int i)
{
  if((c[i].instr=="L.D")||(c[i].instr=="S.D"))
                    for(int j=0;j<5;j++)
                    {
                    pipe[i].stages[j]=a.stld[j];
                    }
 if((c[i].instr=="ADD.D")||(c[i].instr=="SUB.D"))
                    for(int j=0;j<8;j++)
                    {pipe[i].stages[j]=a.addsub[j];}
if(c[i].instr=="MUL.D")
                    for(int j=0;j<11;j++)
                   pipe[i].stages[j]=a.mult[j];
}
int check_previnst(int i)
{
  string s;
  if((c[i].instr=="S.D")||(c[i].instr=="L.D"))
   s="MEM";
  if((c[i].instr=="SUB.D")||(c[i].instr=="ADD.D"))
   s=" A4";
  if((c[i].instr=="MUL.D"))
   s=" M7";
  int c=0;
  for(int k=0;c==0;k++)
  {
    if(pipe[i].stages[k]==s)
    {
      return k;
}
  }

}
int number_stalls(int i,int y)
{
int x;
x=check_previnst(i);//check for previous inst's mem or exe

if(y>x)
{
 return 0;}
else
{
return x;}

}
int check_currentinst(int i)
{
int d=check_id(i,i-1);
if((c[i].instr=="L.D")||(c[i].instr=="S.D"))
   {
   return d+3;}
if((c[i].instr=="ADD.D")||(c[i].instr=="SUB.D")||(c[i].instr=="MUL.D"))
  {
  return d+2;}
}
int check_dependency(int i)
{
int a[10],p,temp;
a[0]=0;
a[1]=0;
int x=0;
int y=check_currentinst(i);
for(int j=0;j<i;j++)
 {
  if((c[j].reg1==c[i].reg1)||(c[j].reg1==c[i].reg2)||(c[j].reg1==c[i].reg3))
  {
    p=number_stalls(j,y);
    if(p!=0)
    {if(c[j].reg1==c[i].reg1)
      {a[x]=++p;
      x++;}
      else{a[x]=p;
      x++;}
    }
  }}
  temp=a[0];
  if(x>=1)
  for(int o=1;o<x;o++)
  {
  if(a[o]>a[o-1])
    temp=a[o];
  }
   return temp;
  }

int instrcall()
{
   for(int i=0;i<codelength;i++)
    {
    if(i==0)
    pipelining1(i);
    else
    {
    int k=check_id(i,i-1);//check for giving next if
    int l=check_dependency(i);
    pipelining(i,k,l);
    }
    }
}
void write()
{
     cout<<"\nEnter the filename to save the timing diagram\n";
     string timingfile;
     cin>>timingfile;
     ofstream outputfile;
     outputfile.open(timingfile.c_str());
     outputfile<<"THE TIMING DIAGRAM FOR THE GIVEN CODE:"<<endl<<endl<<endl;
     int c;
     for(int i=0;c<codelength;i++)
     {if(i!=0)
      outputfile<<"c#"<<i<<"\t";
else
outputfile<<"   "<<"\t";
for(int t=0;t<codelength;t++)
{if(i==0)
 outputfile<<"I#"<<t+1<<"\t";
 else
 {
 outputfile<<pipe[t].stages[i-1]<<"\t";
 if(pipe[t].stages[i-1]==" WB")
  c++;
 }}
 outputfile<<"\n";
}
outputfile.close();
}
void compute()
{
  for(int i=0;i<codelength;i++)                   //seperating ',' from the values
  {
      if((c[i].instr=="L.D")||(c[i].instr=="S.D"))
      {
          c[i].reg1.erase(c[i].reg1.end()-1);
          c[i].reg2.erase(c[i].reg2.end()-1);
          string s;
          if(c[i].instr=="L.D")
          {
            string s1,s2;
            int n1;
            stringstream sw(c[i].reg2);
            getline(sw,s1,'(');
            getline(sw,s2,')');
            sw.str("");
			sw.clear();
            stringstream ss;
			ss<<s1;
			ss>>n1;
			ss.str("");
			ss.clear();
			int r=regs.find(s2)->second;
            int n=n1+r;
            float m=mem.find(n)->second;
            int w=fpregs.count( c[i].reg1 );
            if(w==0)
              fpregs[c[i].reg1]=m;
            else
              fpregs.find(c[i].reg1)->second=m;
            }
            if(c[i].instr=="S.D")
          {
            float f;
            f=fpregs.find(c[i].reg2)->second;
            string s1,s2;
            int n1;
            stringstream sw(c[i].reg1);
            getline(sw,s1,'(');
            getline(sw,s2,')');
            sw.str("");
			sw.clear();
            stringstream ss;
			ss<<s1;
			ss>>n1;
			ss.str("");
			ss.clear();
			int r=regs.find(s2)->second;
            int n=n1+r;
            mem.find(n)->second=f;
          }
      }
      if((c[i].instr=="ADD.D")||(c[i].instr=="SUB.D")||(c[i].instr=="MUL.D"))
      {
          c[i].reg1.erase(c[i].reg1.end()-1);
          c[i].reg2.erase(c[i].reg2.end()-1);
          c[i].reg3.erase(c[i].reg3.end()-1);
          float f1,f2,f3;
          f2= fpregs.find(c[i].reg2)->second;
          f3= fpregs.find(c[i].reg3)->second;
          if (c[i].instr=="ADD.D")
              f1=f2+f3;
          if (c[i].instr=="SUB.D")
              f1=f2-f3;
          if (c[i].instr=="MUL.D")
              f1=f2*f3;
          int w=fpregs.count( c[i].reg1 );
          if(w==0)
              fpregs[c[i].reg1]=f1;
          else
              fpregs.find(c[i].reg1)->second=f1;
      }
  }
}
void writefp()
{
     cout<<"\nenter the filename to save the information about the register contents\n";
     string registerfile;
     cin>>registerfile;
     ofstream outputfile;
     outputfile.open(registerfile.c_str());
     outputfile<<"THE FLOATING POINT REGISTERS USED IN THE CODE AND THEIR CORRESSPONDING VALUES:"<<endl<<endl;
     map<string, float>::iterator fiter;
     for( fiter = fpregs.begin(); fiter != fpregs.end(); fiter++ )
    {
     outputfile<< fiter->first << "\t";
    }
    outputfile<<endl;
    for( fiter = fpregs.begin(); fiter != fpregs.end(); fiter++ )
    {
     outputfile<< fiter->second << "\t";
    }
    outputfile.close();
}

int main()
{
    for(int j=0;j<200;j++)
      for(int i=0;i<100;i++)
        pipe[j].stages[i]="   ";
    cout << "Enter the input filename:" << endl; // prints
    char s[100];
    cin>>s;
    readfile(s);
    //cout<<"\nthe total length of the code is "<<codelength;
    instrcall();
    write();
    compute();
    writefp();
    return 0;
}




