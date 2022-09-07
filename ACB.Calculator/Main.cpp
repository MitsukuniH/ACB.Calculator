#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Calculator
{
public:
	Calculator();
	void ReceiveInput();
	void ProcessInput();
private:
	char GetVarName(const string& formula);
	int CalculateInt(const string& formula);
	vector<int> CalculateArray(const string& formula);

	int mN;
	vector<pair<string, string>> mCommands;
	unordered_map<char, int> mVars;
	unordered_map<char, vector<int>> mArrays;
};


int main()
{
	Calculator calculator;

	calculator.ReceiveInput();
	calculator.ProcessInput();

	return 0;
}

Calculator::Calculator() :mN(0) 
{}

void Calculator::ReceiveInput()
{
	cin >> mN;
	mCommands.resize(mN);

	for (auto& com : mCommands)
	{
		cin >> com.first;
		char c = 0;
		while (c != ';')
		{
			c = getchar();
			com.second += c;
		}
	}
}

void Calculator::ProcessInput()
{
	for (auto& com : mCommands)
	{
		if (com.first == "int")
		{
			mVars.emplace(
				GetVarName(com.second),
				CalculateInt(com.second)
			);
		}
		else if (com.first == "print_int")
		{
			cout << CalculateInt(com.second) << endl;
		}
		else if (com.first == "vec")
		{
			mArrays.emplace(
				GetVarName(com.second),
				CalculateArray(com.second)
			);
		}
		else if (com.first == "print_vec")
		{
			vector<int> outVec = CalculateArray(com.second);
			cout << "[ ";
			for (int cont:outVec) {
				cout << cont << " ";
			}
			cout << "]" << endl;
		}
	}
}

char Calculator::GetVarName(const string& formula)
{
	for (char member : formula)
	{
		if (member >= 'a' && member <= 'z')
		{
			return member;
		}
	}
	return '.';
}

int Calculator::CalculateInt(const string& formula)
{
	int result = 0;
	bool doSum = true;
	for (char member : formula)
	{
		switch (member)
		{
		case '=': result = 0; break;
		case ' ': break;
		case '+': doSum = true; break;
		case '-': doSum = false; break;
		default:
			if (member >= '0' && member <= '9')
			{
				doSum ? result += (member - '0')
					: result -= (member - '0');
				//cout << result << "const" << endl;
			}
			else if (mVars.count(member))
			{
				doSum ? result += mVars.at(member)
					: result -= mVars.at(member);
				//cout << result << member << "." << endl;
			}
			//cout << vars.count(member) << endl;
			break;
		}
	}
	//cout << result << endl;
	return result;
}

vector<int> Calculator::CalculateArray(const string& formula)
{
	vector<int> result;
	vector<int> vecMem;
	bool doSum = true, isFirst = false;
	int index = 0;
	for (char member : formula)
	{
		switch (member)
		{
		case '=': result.clear(); break;
		case ' ': break;
		case '+': doSum = true; break;
		case '-': doSum = false; break;
		case '[': index = 0; break;
		case ',': ++index; break;
		case ']': isFirst = false; break;
		default:
			if (member >= '0' && member <= '9')
			{
				if (result.empty())isFirst = true;
				if (isFirst)result.emplace_back(member - '0');
				else
				{
					doSum ? result.at(index) += (member - '0')
						: result.at(index) -= (member - '0');
					//cout << index << ":" << result.at(index) << endl;
				}
				
			}
			else if (mVars.count(member))
			{
				if (result.empty())isFirst = true;
				if (isFirst)result.emplace_back(mVars.at(member));
				else
				{
					doSum ? result.at(index) += mVars.at(member)
						: result.at(index) -= mVars.at(member);
				}
			}
			else if (mArrays.count(member))
			{
				if (result.empty())result = mArrays.at(member);
				else
				{
					for (int i = 0;i < result.size();++i)
					{
						doSum ? result.at(i) += mArrays.at(member).at(i) 
							: result.at(i) -= mArrays.at(member).at(i);
					}
				}
			}
			//cout << vars.count(member) << endl;
			break;
		}
	}
	//cout << result << endl;
	return result;
}
