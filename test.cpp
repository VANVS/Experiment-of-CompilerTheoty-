#include "RDParser.h"
#include "LexicalAnalyzer.h"
#include <iostream>

int main()
{
	LexicalAnalyzer analyzer;
	analyzer.start();
	std::cout << "�ʷ�������ɣ�\n";
	RDParser parser;
	parser.start();
	std::cout << "�﷨������ɣ�\n";
	system("pause");
	return 0;
}