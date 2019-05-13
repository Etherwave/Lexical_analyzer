#include<iostream>
#include<string.h>
#include<string>
#include<map>
#include<stack>
#include<algorithm>
#include <stdlib.h>
#include<iomanip>

using namespace std;

/*
1	2	3	4	5	6	7	8	9	10
int	char	float	void	const	if	else	do	while	scanf
11	12	13	14	15	16	17	18	19	20
printf	return	main	read	+	-	*	/	%	=
21	22	23	24	25	26	27	28	29	30
= =	>	<	£¡=	>=	<=	&&	||	!	<>
31	32	33	34	35	36	37	38	39	40
(	)	{	}	£»	,	"	'	++	--

*/

map<int, string> table[5];

class Binary_form
{
public:
	int classNum, id;
};

ostream &operator << (ostream& out, const Binary_form& a)
{
	string className = "";
	switch (a.classNum)
	{
		case 0:className = "$ReserveWord"; break;
		case 1:className = "$IdentifierWord"; break;
		case 2:className = "$ConstWord"; break;
		case 3:className = "$OperatorWord"; break;
		case 4:className = "$DelimiterWord"; break;
		default:break;
	}
	out <<setw(15)<< className << " " << table[a.classNum][a.id];
	//out << setw(5) << a.classNum << " " << a.id;
	return out;
}

class Lexical_analyzer
{
public:
	string classesName[5] = { "$ReserveWord","$IdentifierWord",
	"$ConstWord","$OperatorWord","$DelimiterWord" };

	//词类种树 
	const static int classNum = 5;

	map<string, int> classes[classNum];

	char ch;

	string strToken;

	//由于界符的使用，会导致在\"或\'号后边的值为常量，
	//那么我们需要标记一下，下一个读的字符是否在这两种界符之间
	//如果在\"之后我们标记为1
	//如果在\'之后我们标记为3
	//不在标记为0
	//由于字符串检测不到结束的引号，那么就会一直往后读入，直到读入一个\"表示我们该字符串结束了
	//但字符只能读入一个字符，我们读完一个字符来，判断下一个是不是\'，如果不是那么报错，
	//可知我们词法分析器每次读入一个符",第二次读入hello，第三次读入"
	//可知第一次读入时我们的DelimiterType为0
	//第二次我们读入时我们的DelimiterType为1
	//第三次我们读入时我们的DelimiterType为2

	//同样我们设置读入'a'时DelimiterType 的变化为0 3 4
	int DelimiterType;

	Lexical_analyzer()
	{
		Init();
	}

	//有关保留字，操作符，界符的定义初始化
	void InitClasses()
	{
		//$ReserveWord
		this->classes[0]["int"] = 1;
		this->classes[0]["char"] = 2;
		this->classes[0]["float"] = 3;
		this->classes[0]["void"] = 4;
		this->classes[0]["const"] = 5;
		this->classes[0]["if"] = 6;
		this->classes[0]["else"] = 7;
		this->classes[0]["do"] = 8;
		this->classes[0]["while"] = 9;
		this->classes[0]["scanf"] = 10;
		this->classes[0]["printf"] = 11;
		this->classes[0]["return"] = 12;
		this->classes[0]["main"] = 13;
		this->classes[0]["read"] = 14;

		//$OperatorWord
		this->classes[3]["+"] = 1;
		this->classes[3]["-"] = 2;
		this->classes[3]["*"] = 3;
		this->classes[3]["/"] = 4;
		this->classes[3]["%"] = 5;
		this->classes[3]["="] = 6;
		this->classes[3]["=="] = 7;
		this->classes[3][">"] = 8;
		this->classes[3]["<"] = 9;
		this->classes[3]["!="] = 10;
		this->classes[3][">="] = 11;
		this->classes[3]["<="] = 12;
		this->classes[3]["&&"] = 13;
		this->classes[3]["||"] = 14;
		this->classes[3]["!"] = 15;
		this->classes[3]["<>"] = 16;
		this->classes[3]["++"] = 17;
		this->classes[3]["--"] = 18;
		this->classes[3]["&"] = 19;
		this->classes[3]["|"] = 20;

		//$DelimiterWord
		this->classes[4]["("] = 1;
		this->classes[4][")"] = 2;
		this->classes[4]["{"] = 3;
		this->classes[4]["}"] = 4;
		this->classes[4][";"] = 5;
		this->classes[4][","] = 6;
		this->classes[4]["\""] = 7;
		this->classes[4]["\'"] = 8;
	}

	void Init()
	{
		for (int i = 0; i != classNum; i++)
		{
			this->classes[i].clear();
		}
		//初始化时，使界符无效
		DelimiterType = 0;
		//初始化保留字，操作符，界符
		InitClasses();
		//缓冲的字符串置为空
		strToken = "";
	}

	void GetBC()
	{
		while (cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t')cin.get();
	}

	bool GetChar()
	{
		ch = cin.peek();
		if (ch == EOF)return false;
		return true;
	}

	void Concat()
	{
		ch = cin.get();
		strToken += ch;
	}

	bool IsLetter()
	{
		if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')return true;
		return false;
	}

	bool IsDigit()
	{
		if (ch >= '0' && ch <= '9')return true;
		return false;
	}
	
	bool IsOperator()
	{
		string tch = "";
		tch += ch;
		if (classes[3].find(tch) != classes[3].end())return true;
		return false;
	}

	bool isDelimiter()
	{
		string tch = "";
		tch += ch;
		if (classes[4].find(tch) != classes[4].end())return true;
		return false;
	}

	bool IsStringEnd()
	{
		if (ch != '\"')return false;
		if (strToken.size() == 1 && strToken[strToken.size() - 1] == '\\')return false;
		if (strToken.size() > 1 
			&& strToken[strToken.size() - 1] == '\\' && strToken[strToken.size() - 2] != '\\')return false;
		return true;
	}

	bool IsCharEnd() 
	{
		return ch == '\'';
	}

	int GetReserveId()
	{
		return classes[0][strToken];
	}

	int InsertId()
	{
		if (classes[1][strToken] == 0)
		{
			classes[1][strToken] = classes[1].size() + 1;
			table[1][classes[1][strToken]] = strToken;
		}
		return classes[1][strToken];
	}

	int InsertConst()
	{
		if (classes[2][strToken] == 0)
		{
			classes[2][strToken] = classes[2].size() + 1;
			table[2][classes[2][strToken]] = strToken;
		}
		return classes[2][strToken];
	}

	int GetOperatroId()
	{
		return classes[3][strToken];
	}

	int GetDelimiterId()
	{
		return classes[4][strToken];
	}

	void OutTable()
	{
		for (int i = 0; i != 5; i++)
		{
			map<string, int>::iterator it;
			for (it = classes[i].begin(); it != classes[i].end(); it++)
			{
				cout << it->first << " " << it->second << endl;
			}
		}
	}

	Binary_form ReadAWord()
	{
		Binary_form aform;
		strToken = "";
		//如果当前是字符或字符串输入模式，那么我们不能直接去掉空格
		if(DelimiterType==0)GetBC();
		GetChar();
		//如果DelimiterType!=0那么我们即将处理有关字符或字符串的东西，与其他类型不同，我们单独处理

		if (DelimiterType != 0)
		{
			switch (DelimiterType)
			{
				case 1:
				{
					//如果当前状态为输入字符串，那么我们就一直读入字符一直到遇到\"
					while (!IsStringEnd())
					{
						Concat();
						GetChar();
					}
					aform.classNum = 2;
					aform.id = InsertConst();
					DelimiterType = 2;
				}break;
				case 2:
				{
					//我们读取完一个字符串后，并且碰到了一个不是转移字符的\"时才会将DelimiterType置为2
					//那么这就是读入一个\"
					Concat();
					aform.classNum = 4;
					aform.id = GetDelimiterId();
					//重置DelimiterType=0
					DelimiterType = 0;
				}break;
				case 3:
				{
					//我们此时是要读入一个字符
					Concat();
					GetChar();
					if (strToken == "\\")
					{
						Concat();
					}
					if (IsCharEnd()) 
					{
						aform.classNum = 2;
						aform.id = InsertConst();
						DelimiterType = 4;
					}
					else
					{
						aform.classNum = 0;
						aform.id = 0;
						DelimiterType = 0;
					}
				}break;
				case 4:
				{
					//可知我们是从DelimiterType=3转过来的
					//我们现在就只需要读入那个\'
					Concat();
					aform.classNum = 4;
					aform.id = GetDelimiterId();
					DelimiterType = 0;
				}break;
				default:break;
			}
		}
		else
		if (IsLetter())
		{
			//不断读取字符或数字
			while (IsLetter() || IsDigit())
			{
				Concat();
				GetChar();
				GetBC();
			}

			//看看这个标识符是不是保留字，如果是那么就返回非0正数，否则返回0
			int flag = GetReserveId();

			//如果不是保留字
			if (flag == 0)
			{
				aform.classNum = 1;
				aform.id = InsertId();
			}
			else
			{
				//如果是保留字，那么我们直接返回对应的类别和ID
				aform.classNum = 0;
				aform.id = flag;
			}
		}
		else
		if (IsDigit())
		{
			//由于我们使用了float，那么可能会有小数点，但是小数点只能出现一次
			//那么我尝试将float分两次读入,第一次读入小数点前边的部分
			//第二次读入小数点后边的部分
			
			while (IsDigit())
			{
				Concat();
				GetChar();
				GetBC();
			}
			if (ch == '.')
			{
				Concat();
				GetChar();
				GetBC();
				while (IsDigit())
				{
					Concat();
					GetChar();
					GetBC();
				}
			}

			aform.classNum = 2;
			aform.id = InsertConst();
		}
		else
		if (IsOperator())
		{
			while (IsOperator())
			{
				Concat();
				GetChar();
				GetBC();
			}
			aform.classNum = 3;
			aform.id = GetOperatroId();
		}
		else
		if (isDelimiter())
		{
			Concat();
			aform.classNum = 4;
			aform.id = GetDelimiterId();
			//如果界符是\"，那么我们使DelimiterType有效为1
			if (aform.id == 7)
			{
				DelimiterType = 1;
			}
			else
			if (aform.id==8)
			{
				//如果界符是\'，那么我们使DelimiterType有效为3
				DelimiterType = 3;
			}
		}
		else
		{
			aform.classNum = 0;
			aform.id = 0;
		}
		return aform;
	}
};

void InitTable()
{
	for (int i = 0; i != 5; i++)
	{
		table[i].clear();
	}

	//$ReserveWord
	table[0][1] = "int";
	table[0][2] = "char";
	table[0][3] = "float";
	table[0][4] = "void";
	table[0][5] = "const";
	table[0][6] = "if";
	table[0][7] = "else";
	table[0][8] = "do";
	table[0][9] = "while";
	table[0][10] = "scanf";
	table[0][11] = "printf";
	table[0][12] = "return";
	table[0][13] = "main";
	table[0][14] = "read";

	//$OperatorWord
	table[3][1] = "+";
	table[3][2] = "-";
	table[3][3] = "*";
	table[3][4] = "/";
	table[3][5] = "%";
	table[3][6] = "=";
	table[3][7] = "==";
	table[3][8] = ">";
	table[3][9] = "<";
	table[3][10] = "!=";
	table[3][11] = ">=";
	table[3][12] = "<=";
	table[3][13] = "&&";
	table[3][14] = "||";
	table[3][15] = "!";
	table[3][16] = "<>";
	table[3][17] = "++";
	table[3][18] = "--";
	table[3][19] = "&";
	table[3][20] = "|";

	//$DelimiterWord
	table[4][1] = "(";
	table[4][2] = ")";
	table[4][3] = "{";
	table[4][4] = "}";
	table[4][5] = ";";
	table[4][6] = ",";
	table[4][7] = "\"";
	table[4][8] = "\'";
}

int main()
{
	freopen("in.cpp", "r", stdin);

	InitTable();

	Lexical_analyzer la;
	la.OutTable();
	Binary_form aform=la.ReadAWord();
	while (aform.id)
	{
		cout << aform << endl;
		aform = la.ReadAWord();
	}
	return 0;
}