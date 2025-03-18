#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include <cctype> 
#include<bits/stdc++.h> 
using namespace std;
int adress = 0;
typedef struct{
    string opcode ="",operand_1 ="",operand_2="",operand_3="";
    int address;
}instr;
int offset_addr;
// typedef struct{
//     int opcode,operand_1,opernad_3;
// }int_instr;
//  typedef struct{
//     int opcode,RorI =00, mo=00,operand_1=0000,operand_2=0000,operand_3=0000;
//  }instr;
vector<string>encoding;
vector<instr>instruction;
map<string,int>label;
map<string,string>divider;
map<char, string> hextobinary = {
    {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
    {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
    {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
    {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
};
map<string, string> opcodeTable = {
    {"add", "00000"}, {"sub", "00001"}, {"mul", "00010"}, {"div", "00011"},
    {"mod", "00100"}, {"cmp", "00101"}, {"and", "00110"}, {"or", "00111"},
    {"not", "01000"}, {"mov", "01001"}, {"lsl", "01010"}, {"lsr", "01011"},
    {"asr", "01100"}, {"nop", "01101"}, {"ld", "01110"}, {"st", "01111"},
    {"beq", "10000"}, {"bgt", "10001"}, {"b", "10010"}, {"call", "10011"},
    {"ret", "10100"},{"addu",""},{"subu",""},{"mulu",""},{"divu",""},{"modu",""},
    {"cmpu",""},{"andu", ""}, {"oru", ""},
    {"notu", ""}, {"movu", ""}, {"lslu", ""}, {"lsru", ""},
    {"asru", ""},{"addh",""},{"subh",""},{"mulh",""},{"divh",""},{"modh",""},
    {"cmph",""},{"andh", ""}, {"orh", ""}, {"noth", ""}, {"movh", ""}, {"lsl h", ""}, {"lsrh", ""},
    {"asrh", ""},{"hlt","11111"},
};
map<string,string>modifiers={{"u","01"},{"h","10"},{"o","00"}};
 map<string, string> registers = {
                    {"r0", "0000"}, {"r1", "0001"}, {"r2", "0010"}, {"r3", "0011"},
                    {"r4", "0100"}, {"r5", "0101"}, {"r6", "0110"}, {"r7", "0111"},
                    {"r8", "1000"}, {"r9", "1001"}, {"r10", "1010"}, {"r11", "1011"},
                    {"r12", "1100"}, {"r13", "1101"}, {"r14", "1110"}, {"r15", "1111"},{"","9999"}
};
map<string,string>three_operands = {{"add", "00000"}, {"sub", "00001"}, {"mul", "00010"}, {"div", "00011"}, {"mod", "00100"},
                                    {"and", "00110"}, {"or", "00111"},{"asr", "01100"},{"lsl", "01010"}, {"lsr", "01011"},
                                    {"addu",""},{"subu",""},{"mulu",""},{"divu",""},{"modu",""},{"andu", ""},
                                     {"oru", ""},{"lslu", ""}, {"lsru", ""},{"asru", ""},{"ld", "01110"}, {"st", "01111"},
                                     {"subh",""},{"mulh",""},{"divh",""},{"modh",""},
                                     {"cmph",""},{"andh", ""}, {"orh", ""}, {"noth", ""}, {"movh", ""}, {"lsl h", ""}, {"lsrh", ""},
                                     {"asrh", ""}};

map<string,string>two_operands = {{"cmp", "00101"}, {"not", "01000"}, {"mov", "01001"},
                                {"cmpu",""} , {"movu", ""}, {"notu", ""}};
map<string,string>no_operands ={{"nop", "01101"}, {"ret", "10100"},{"hlt","11111"}};
map<string,string>one_operands= {{"beq", "10000"}, {"bgt", "10001"}, {"b", "10010"}, {"call", "10011"}};

// TASK :
// read from fil
// pase it and store in proper memory storage 
// write it to a file 
string dec2complement(int num, int bits) {
    return bitset<32>(num).to_string().substr(32 - bits);
}
string trim(string s) {
    // Remove leading spaces
   int i=0;
   while(i<s.size() && isspace(s[i]))
   {
        i++;
   }
   s.erase(0,i);
   i=s.size()-1;
   while(i>=0 && isspace(s[i]))
   {
        i--;
   }
   s.erase(i+1);

    return s;
}
int constant_checking(string s)
{
        //cout<<s<<endl;

    for(int i=0;i<s.size();i++)
    {
        if(s[0]=='0'&& s[1]=='x')
        {
            //cout<<"ok \n";
        }
        else{
            return 0;
            break;
            
        }
        if(i>1){
           char chk = s[i];
            if((chk>=48 && chk<=57) || (chk>=65 && chk<=70) || (chk>=97 && chk<=102) ){
                if(i>6)
                {
                    cout<<"the value is exceeding 16 bit"<<endl;
                    return 0;
                }
                else{
                    continue;
                }
            }else{
               cout<<"invalid immediate value "<<endl;
                    return 0;
            }
        }
       
        
    }  
    return 1;
}

bool encode(int in,instr s)
{
    string r = "";
   // cout<<"it is reading\n";
    //encoding for three operand instructions 
        if(in == 3)
    {
        string operand_1;
        string operand_2;
        string operand_3;
        string modi ="" ;
        string temp="";
        int c=0;
        if(s.opcode.size()>2 && s.operand_3 == "")
        {
            cout << "incorrectt number of operands\n";
            return false ;
        }
        if(s.operand_2.find('[')!=string::npos)
        {
            string change = s.operand_2;
            s.operand_2.clear();
           // cout<<"golden parrow"<<endl;
            for(int i=0;i<change.size()-1;i++)
            {
                if(change[i]!='[')
                {
                    switch(c)
                    {
                        case 0:
                            s.operand_3+=change[i];
                            break;
                        case 1:
                            s.operand_2+=change[i];
                    }
                }
                else {
                    c++;
                }
            }
            // cout<<"operand_2 is"<<s.operand_2<<endl;
            // cout<<"operand_2 is"<<s.operand_3<<endl;
        }
        if (s.opcode.size() == 4)
        {
            modi += s.opcode[3];
            s.opcode.erase(3,3);
            r+=opcodeTable.find(s.opcode)->second; 
            if(registers.find(s.operand_3)!=registers.end())
            {
                cout<<"the eneterd instruction is wrong";
                return false;
            }

        }
        else if (s.opcode.size() == 2)
        {
            modi="o";
            r+=opcodeTable.find(s.opcode)->second; 
         }else{
            r+=opcodeTable.find(s.opcode)->second;
        }
      
        //r+='|';
       // cout<<r<<endl;
        char r_type;
        
         if(registers.find(s.operand_3)!=registers.end() )
        {
            r_type='0';
            temp+=registers.find(s.operand_3)->second;
           // temp+="|";
            temp+="00000000000000";
        }
        else if (constant_checking(s.operand_3))
        {
            s.operand_3.erase(0,2);
            int size = s.operand_3.size();
            //cout<<s.operand_3<<endl;
            temp+=modifiers.find(modi)->second;
           // temp+="|";
            for(int i=0 ; i<4*(4-size);i++)
                {
                    temp+='0';
                }
              for(int i=0;i<s.operand_3.size();i++)
              {
                auto it= hextobinary.find(s.operand_3[i]);
                //cout<<s.operand_3.size()<<endl;
                if(it != hextobinary.end()){
                    temp+=it->second;
                    }
                    else{
                       cout<<"there is a problem in your immediate value for the instruction"<<s.opcode<<s.operand_1<<s.operand_2<<s.operand_3<<endl;
                        return false ;
                        break ;

                    }
                   
              } 
             r_type ='1';
        }
        else{
            cout<<"your immediate value for the instruction is wrong or you should enteer the three operands for this instruction\n ";
            return false;
        }
        r+=r_type;
        //r+='|';
       //cout<<r<<"taken the character "<<endl;
        if(registers.find(s.operand_1)!=registers.end()){
            r+=registers.find(s.operand_1)->second;
            //r+='|';
            //cout<<r<<endl;
        }
        else{
            cout<<"you can enter only valid registers  "<<endl;
            return false;
        }
        if(registers.find(s.operand_2)!=registers.end()){
                 r+=registers.find(s.operand_2)->second;
                 //cout<<r<<endl;
                 //r+='|';
        }
        else{
           cout<<"you can enter only valid registers  "<<endl;
            return false;

        }

        r+=temp;
        //r+='|';
        //cout<<r<<endl;
        encoding.push_back(r);
        return true;
    }
    if(in == 2)
    {
        string operand_1;
        string operand_2;

        string opcode;
        string type;
        string modi;
        string temp;
        char r_type;
        if(s.operand_2 == "")
        {
            cout<<"incorrect number of operands \n";
            return false;
        }
        if(s.operand_3!="")
        {
             cout<<"you cant enetr three operands for two operand instruction";
            return false ;
        }
        s.operand_3="0000";
        if (s.opcode.size() == 4)
        {
            modi += s.opcode[3];
            s.opcode.erase(3,3);
            opcode+=opcodeTable.find(s.opcode)->second; 
        }
        else{
           opcode+=opcodeTable.find(s.opcode)->second;
           modi = "o";
          
        }
        //opcode+='|';
        //cout<<r<<endl;        
            if(registers.find(s.operand_2)!=registers.end() )
            {
                type="0";
                temp+=registers.find(s.operand_2)->second;
                temp+="00000000000000";
            }
            else if (constant_checking(s.operand_2))
            {
                //cout<<"evadra veedu"<<endl;
                s.operand_2.erase(0,2);
                int size = s.operand_2.size();
                //cout<<s.operand_2<<endl;

                temp+=modifiers.find(modi)->second;
              //  temp+="|";
                for(int i=0 ; i<16-4*size;i++)
                    {
                        temp+='0';
                    }
                  for(int i=0;i<s.operand_2.size();i++)
                  { 
                    auto it= hextobinary.find(s.operand_2[i]);
                    //cout<<s.operand_2.size()<<endl;
                    if(it != hextobinary.end()){
                        temp+=it->second;
                        }
                        else{
                            cout<<"there is a problem in your immediate value for the instruction"<<s.opcode<<s.operand_1<<s.operand_2<<s.operand_3<<endl;
                            return false ;
                            break ;
    
                        }
                       
                  } 
                 type ="1";
            }
            else{
                cout<<"your immediate value for the instruction is wrong\n";
                return false;
            }
            if(registers.find(s.operand_1)!=registers.end()){
                operand_1=registers.find(s.operand_1)->second;
                //cout<<operand_1<<endl;
            }
            else{
               cout<<"you can enter only valid registers  "<<endl;
                return false;
            }
            if(s.opcode == "cmp" )
        {
            r+=opcode;
            r+=type;
         //   r+="|";
            r+=s.operand_3;
          //  r+="|";
            r+=operand_1;
          //  r+="|";
            r+=temp;
        //    cout<<r<<endl;
        //    cout<<r.size()<<endl;
       
        }
        else if (s.opcode == "mov" || s.opcode == "not")
        {
            r+=opcode;
            r+=type;
           // r+="|";
            r+=operand_1;
          //  r+="|";
            r+=s.operand_3;
          //  r+="|";
            r+=temp;
          // cout<<r<<endl;
          // cout<<r.size()<<endl;



        }
       
    }


    if(in==1)
    {
        int im;
        if(label.find(s.operand_1)!=label.end())
        
        {
            im =label.find(s.operand_1)->second;
        }
        else{
           cout<<"give in an incorrect label at "<<s.operand_2<<endl;
           return false;
        }
        r+=opcodeTable.find(s.opcode)->second;
        //r+="|";
        r+=dec2complement(im-s.address,27);
       // cout<<r<<endl;
    }
    if(in==0)
    {
        r+=opcodeTable.find(s.opcode)->second;
        r+="000000000000000000000000000";
        //cout<<r<<endl;
    }
    encoding.push_back(r);

   
}






bool  valid(instr s){
//check opcode 
bool check = true;
        if(opcodeTable.find(s.opcode)!=opcodeTable.end())
        {
            if(three_operands.find(s.opcode)!=three_operands.end())
            {
               check =  encode(3,s);
            }
            else if(two_operands.find(s.opcode)!= two_operands.end())
            {
                check = encode(2,s);
               // cout<<"it is a two operna one \n";
            }
            else if(one_operands.find(s.opcode)!= one_operands.end())
            {
                check = encode(1,s);
            }
            else if(no_operands.find(s.opcode)!= no_operands.end())
            {
                check = encode(0,s);
            }
        }
        else {
            cout<<"there is an incorrect opcode entered in or  "<<s.opcode<<endl;
            check = false ;
        }
        return check ;

}

string parser(string s){
   
    stringstream ss(s);
    string r="";
    string temp;
    string rule;
    int round=0;
    
    // if(s.find(':')!=string::npos)
    // {
    //     getline(ss,temp,':');
    //     temp = trim(temp);
    //     if(temp == "start")
    //     {
    //         offset_addr = adress;
    //     }
    //     label.insert({temp,adress});
    //     getline(ss,rule,';');
    //     rule = trim(rule);
    //     if(rule == " ")
    //     {
    //         cout<<"this is our value |"<<rule[0]<<"now is your time"<<endl;
    //         rule="";
    //     }
    //     else {        s = rule;}
    // }
  //  cout<<"this rule "<<s<<endl;
    int i = s.length();
    int ns =0;

    for (int j=0; j<i;j++)
    {
        if(s[j]==' ' && ns==0 )
        {
            if(j==0)
            {
                cout<<"errror"<<endl;
                r="errorr";
                break;
            }
            else{
               
                    r+=',';
                    ns++;
                
            }
        }else
            {
                if(s[j]!=' ')
                {
                        r+=s[j];
                }
                
                
            }   
}
   
    return r;
}
string inst(string s)
{
    //cout<<s<<endl;
    stringstream ss(s);
    instr rob;
    string temp;
    int c = 0;

    while (getline(ss, temp, ',')) {
        if (c == 0) rob.opcode = trim(temp);
        else if (c == 1) rob.operand_1 = trim(temp);
        else if (c == 2) rob.operand_2 = trim(temp);
        else if (c == 3) rob.operand_3 = trim(temp);
        c++;
    }
    rob.address=adress;

    instruction.push_back(rob); 
    return temp ;
}void writeHexFile(const vector<string>& binaryStrings, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    for (const string& binaryString : binaryStrings) {
        if (binaryString.length() != 32) {
            cerr << "Invalid binary string length: " << binaryString << endl;
            continue;
        }

        bitset<32> bits(binaryString); // Convert binary string to bitset
        outFile << setw(8) << setfill('0') << hex << uppercase << bits.to_ulong() << endl; // Ensure 8 hex digits
    }

    outFile.close();
   // cout << "Hex file written successfully." << endl;
}


void writeBinaryFile(const vector<string>& binaryStrings, const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    for (const string& binaryString : binaryStrings) {
        outFile.write(binaryString.c_str(), binaryString.size());
        outFile.put('\n');
    }

    outFile.close();
}



int main()
{
    ifstream inputfile("asm.txt");
    string temp;
   
    if(inputfile.is_open())
    {
       // cout<<"file is open \n";
    }
    else{
        cout<<"file is unable to open\n";
    }

    while( getline(inputfile,temp)){
        string temp_1;
        string temp_2;
        string temp_3;

        
       // cout<<adress<<endl;
       // cout<<temp<<endl;
        stringstream ss(temp);
        stringstream sw(temp);
        getline(ss,temp_1,';');
        stringstream sd(temp_1);
        getline(sd,temp_2,':');
        getline(sd,temp_3,';');    
         temp_1 = trim(temp_1);
         temp_2 = trim(temp_2);
         temp_3 = trim(temp_3);
         string label_inside;
         string instruction_inside;
         int c=0;
         if((temp.find(':')!=string::npos) && (temp.find(';')!=string::npos) && (temp.find(':')<=temp.find(';')))
         {
            
             if((temp_1.size()-temp_2.size()) == 1)
             {
                c=1;//label with comment 
                label_inside = temp_2.size();
             }
             else {
                c=2;
                label_inside = temp_2;
                instruction_inside = temp_3;
             }
            
         }
         else if ((temp.find(':')!=string::npos))
         {
            
            label_inside = temp_2;
            c=1;
            // if(temp_1=="start"){
            // offset_addr=adress;
            // label.insert({temp_1,0});
            // }
            // else  {
            // offset_addr = 200;
            // label.insert({temp_1,adress});
            // }
            
         }
         else if((temp.find(';')!=string::npos))
         {
                instruction_inside = temp_1;
                c=3;
         }
         else if((temp.find(';')==string::npos) && (temp.find(':')==string::npos)&& (temp_1!="") ){
            instruction_inside = temp_1;
            c=3;

         }
         switch(c)
         {
            case 1: 
                if(label_inside=="start"){
                offset_addr=adress;
                label.insert({label_inside,0});
                }
                else  {
                offset_addr = 200;
                label.insert({label_inside,adress+1});
                }
              //  cout<<"only label\n";
                break;
            case 2:
            if(label_inside=="start"){
                offset_addr=adress;
                label.insert({label_inside,0});
                }
                else  {
                offset_addr = 200;
                label.insert({label_inside,adress+1});
                }
                     instruction_inside=parser(instruction_inside);
                inst(instruction_inside);
                 adress++;
               // cout<<"instruction and label\n";
                 break;
            case 3:
                instruction_inside=parser(instruction_inside);
                inst(instruction_inside);
                adress++;
               // cout<<"single instruction\n";
                break;
                //cout<<"program end\n";
         }
            // offset_addr=adress;
            // label.insert({temp_1,0});
            // }
            // else  {
            // offset_addr = 200;
            // label.insert({temp_1,adress});
            // }
           
                // Ignore rest of the line
               // cout<<"1"<<endl;
               // cout<<temp;
                
                temp.clear();
                //  temp_1 = parser(temp_1);
                //  if ( temp_1 !="")
                //  {
                //      inst(temp_1);
                //  }
                // else {
                //    cout<<"a value found fishy \n";
                //   continue;
                // }
                // cout<<instruction[instruction.size()-1].opcode<<endl;
                //  cout<<instruction[instruction.size()-1].operand_1<<endl;
                //  cout<<instruction[instruction.size()-1].operand_2<<endl;

    }
    inputfile.close();
    int error_check = 0;
    for(int i=0;i<instruction.size();i++){
    if(valid(instruction[i])){
       // cout<<instruction[i].opcode<<endl;
     //cout<<"instruction is correct\n";
     }else{
    // cout<<"instruction is not valid\n";
        error_check = 1;
        break;

     }
    }
    if(error_check!=1)
    {
        cout<<"NO errors";
    }

    writeHexFile(encoding, "output.hex");
    writeBinaryFile(encoding, "output.txt");
    for (int i = 0; i < encoding.size(); i++) {
        int r=0;
       // cout<<encoding[i]<<endl;
        } // Move to the next line after each element
        return 0;

    }
    
   
   


