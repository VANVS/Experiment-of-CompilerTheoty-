#pragma once
#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

#include <fstream>
#include <string>
#include <map>

#define COLONMATCHERR 1
#define CHARACTERERR 2

typedef std::pair<std::string, int> DPair;

class LexicalAnalyzer {
private:
	std::map<std::string, int> _CodeTable;	//���ű�
	char _character;			//��ǰ��ȡ���ַ�
	int line = 1;				//��ǰ��ȡ��������
	std::ifstream _fin;			//�����ļ���
	std::ofstream _fout;		//�����Ԫʽ�����
	std::ofstream _ferr;		//��������ļ���
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	void start();							//����������
	DPair LexAnalyze();						//�ʷ���������
	void CreatTable();						//������ű�
	int reserve(const std::string &);		//�ж��Ƿ��Ǳ�����
	void error(int type);					//������
	void output(const DPair binseq);		//������

};

#endif // !LEXICALANALYZER_H_
