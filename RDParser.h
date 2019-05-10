#pragma once
#ifndef RDPARSER_H_
#define RDPARSER_H_

#include <fstream>
#include <string>
#include <map>
#include <vector>

#define SYMBOL 10
#define CONSTANT 11

#define REPEATDEFINE 0
#define MISSINGEND 1
#define MISSINGBEGIN 2
#define MISSINGSEMICOLON 3
#define MISSINGTYPE 4
#define MISSINGVAR 5
#define MISSINGFUNC 6
#define MISSSINGFUNCID 7
#define MISSINGLEFTBR 8
#define MISSINGRIGHTBR 9
#define MISSINGARGUMENT 10
#define UNDEDINEVAR 11
#define MISSINGEUQ 12
#define MISSINGCOLON 13
#define MISSINGIF 15
#define MISSINGTHEN 16
#define MISSINGELSE 17
#define MISSINGOPERATE 18

typedef enum { integers } types;

//���������Ա
struct Value {
	std::string _vname;
	std::string _vproc;
	bool _vkind;
	types _vtype;
	int _vlev;
	int _vadr;
};

//���������Ա
struct Process {
	std::string _pname;
	types _ptype;
	int _plev; 
	int _fadr = -1;
	int _ladr;
};

typedef std::pair<std::string, Process> PPair;

class RDParser {
private:
	std::map<std::string, Process> _ProcessTable;	//����������map�����洢
	std::vector<Value> _ValueTable;					//����������vector�����洢

	//�����ļ���
	std::ifstream _fin;			//��ȡdyd�ļ���
	std::ofstream _fvalt;		//������������ļ���
	std::ofstream _fprot;		//������������ļ���
	std::ofstream _ferr;		//��������ļ���
	int _val_len;			//��ǰ��������ĳ��ȣ�Ϊ��ȷ������λ��
	int _level;				//��ǰ���
	int _linecount;			//��ǰ����
	std::string _sym;		//��ǰ��ƥ����ַ���
	int _id;				//��ǰ��ƥ����ַ������ֱ�����ʶ���ʶ���ͳ���
	std::string _current_process_id;	//��ǰ������
	std::string _last_process_id;		//��һ��������

	void advance();			//���¶�ȡһ���ַ�����_sym��
	//�����ķ����﷨��������
	void S();
	void A();
	void A_s();
	void B();
	void B_s();
	void C();
	void D();
	void E();
	void F();
	void G();
	void H();
	void I();
	void I_s();
	void J();
	void J_s();
	void K();
	void L();
	void M();

public:
	RDParser();
	~RDParser();

	void AddToValueTable(std::string vname, bool vkind);	//��ӱ���������������
	void AddToProcessTable(std::string pname);				//��ӹ��̵�����������
	bool Is_in_ValueTable(std::string vname);				//�ж��Ƿ��ڱ���������
	bool Is_in_ProcessTable(std::string pname);				//�ж��Ƿ��ڹ���������

	void error(int type);		//������
	void start();				//��ʼ����
	void PTGetValAdd();			//������������ݱ����������ƹ��������еı���λ����Ϣ
	void ShowValueTable();		//�����������
	void ShowProcessTable();	//�����������
};
#endif // !RDPARSER_H_
