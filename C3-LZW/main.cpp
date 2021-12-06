#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<string.h>
#include<unordered_map>
#include <bitset>

using namespace std;
string convert_int_to_bin(int number)
{
    string result = bitset<12>(number).to_string();
    return result;
}
string convert_char_to_string(const char* pCh, int arraySize) {
    string str;
    if (pCh[arraySize - 1] == '\0') str.append(pCh);
    else for (int i = 0; i < arraySize; i++) str.append(1, pCh[i]);
    return str;
}

string GetInput(string path) {
    ifstream file(path, ios::in | ios::binary | ios::ate);
    streampos size = file.tellg();
    char* memblock = new char[size];
    file.seekg(0, ios::beg);
    file.read(memblock, size);
    file.close();
    return  convert_char_to_string(memblock, size);
}

vector<int> encoding(string s1)
{
    cout << "Encoding\n";
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }
    string p = "", c = "";
    p += s1[0];
    int code = 256;
    vector<int> output_code;
    cout << "String\tOutput_Code\tAddition\n";
    for (int i = 0; i < s1.length(); i++) {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        }
        else {
            cout << p << "\t" << table[p] << "\t\t"
                << p + c << "\t" << code << endl;
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    cout << p << "\t" << table[p] << endl;
    output_code.push_back(table[p]);
    return output_code;
}
string decoding(vector<int> op)
{
    string res;
    unordered_map<int, string> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }
    int old = op[0], n;
    string s = table[old];
    string c = "";
    c += s[0];
    res += s;
    int count = 256;
    for (int i = 0; i < op.size() - 1; i++) {
        n = op[i + 1];
        if (table.find(n) == table.end()) {
            s = table[old];
            s = s + c;
        }
        else {
            s = table[n];
        }
        res += s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        count++;
        old = n;
    }
    return res;
}
string getDataFromFileText(string path) {
    fstream f(path);
    string text;
    if (!f.good())
    {
        cout << "File not found" << endl;
        return text;
    }
    getline(f, text);
    f.close();
    return text;
}
void writeToFileLZW(string path, vector<int> output_code) {
    fstream f;
    f.open(path, ios::out | ios::binary);
    for (int i = 0; i < output_code.size(); i++) {
        // f.write((char*)&output_code[i], sizeof(int));
        f << convert_int_to_bin(output_code[i]);
    }
    f.close();
}
long long getFileSize(string path) {
    ifstream in(path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}
vector<int> getDataFromFileLZW(string path) {
    vector<int> input_code;
    string input = GetInput(path);
    int i = 0;
    int size = getFileSize(path);
    while (i < size) {
        //Extracting 12 bits and converting binary to decimal
        string subinput = input.substr(i, 12);
        bitset<12> binary(subinput);
        input_code.push_back(binary.to_ullong());
        i += 12;
    }
    return input_code;
    /// //// ////
   /* fstream f(path,ios::in|ios::binary);
    if (!f.good())
    {
        cout << "File not found" << endl;
        return input_code;
    }
    long long size = getFileSize(path)/sizeof(int);
    input_code.resize(size);
    for (int i = 0; i < size; i++) {
        f.read((char*)&input_code[i], sizeof(int));
    }
    f.close();
    return input_code;*/
}
void writeToFileTxt(string path, string res) {
    fstream f;
    f.open(path, ios::out);
    f << res;
    f.close();
}
void Output_info_size(string path_input, string path_output) {
    long long a = getFileSize(path_input);
    long long b = getFileSize(path_output);
    cout << "Input size: " << a * 8 << " bits";
    cout << "\nOutput size: " << b * 8 << " bits";
    cout << "\nSpace saved: " << (1.0 * (a - b) / a) * 100 << " %";
}
int main()
{

    //string s = "WYS*WYGWYS*WYSWYSG";
    string s = getDataFromFileText("data.txt");
    vector<int> output_code = encoding(s);
    writeToFileLZW("compressed.lzw", output_code);
    vector<int> input_code = getDataFromFileLZW("compressed.lzw");
    cout << "Output Codes are: ";
    for (int i = 0; i < input_code.size(); i++) {
        cout << input_code[i] << " ";
    }
    cout << endl;
    string res = decoding(input_code);
    writeToFileTxt("out.txt", res);
    Output_info_size("data.txt", "compressed.lzw");

    return 0;
}