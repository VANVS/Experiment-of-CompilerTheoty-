#include "RDParser.h"
#include <iostream>
#include <vector>

//���캯��
RDParser::RDParser()
{
	//�������ļ�
	_fin.open("result.dyd");
	_ferr.open("error.err", std::ios_base::app);
	_fvalt.open("Value table.table", std::ios_base::out);
	_fprot.open("Process table.table", std::ios_base::out);
	if (!_fin.is_open() || !_ferr.is_open() || !_fvalt.is_open() || !_fprot.is_open()) {
		std::cout << "Cannot open file!\n";
	}

	//��ʼ�����ֲ���
	_val_len = 0;
	_level = 0;
	_linecount = 1;
	_current_process_id = "main";
	AddToProcessTable(_current_process_id);
}

RDParser::~RDParser()
{
}

//��ӱ���������������
void RDParser::AddToValueTable(std::string vname, bool vkind)
{
	if (!Is_in_ValueTable(vname)) {
		Value new_value;
		new_value._vname = vname;
		new_value._vproc = _current_process_id;
		new_value._vkind = vkind;
		new_value._vlev = _level+1;
		new_value._vadr = _val_len;
		_ValueTable.push_back(new_value);
		_val_len++;
	}
	else {
		error(REPEATDEFINE);
	}
}

//��ӹ��̵�����������
void RDParser::AddToProcessTable(std::string pname)
{
	Process new_process;
	new_process._pname = pname;
	new_process._plev = _level+1;

	_ProcessTable[pname] = new_process;
}

//�ж��Ƿ��ڱ���������
bool RDParser::Is_in_ValueTable(std::string vname)
{
	int size = _ValueTable.size();
	for (int i = 0; i < size; i++) {
		if (_ValueTable[i]._vname == vname && _ValueTable[i]._vproc == _current_process_id)
			return true;
	}
	return false;
}

//�ж��Ƿ��ڹ���������
bool RDParser::Is_in_ProcessTable(std::string pname)
{
	std::map<std::string, Process>::iterator it;
	it = _ProcessTable.find(pname);
	if (it != _ProcessTable.end()) {
		return true;
	}
	return false;
}

//�����������ݴ�����Ϣ���б���
void RDParser::error(int type)
{
	switch (type)
	{
	case REPEATDEFINE:
		_ferr << "***LINE:" << _linecount << "  " << _sym << "�ظ�����\n";
		break;
	case MISSINGEND:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٹؼ���end\n";
		break;
	case MISSINGBEGIN:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٹؼ���begin\n";
		break;
	case MISSINGSEMICOLON:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٷֺ�\';\'\n";
		break;
	case MISSINGTYPE:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ������˵����" << _sym << "���ǹؼ���\n";
		break;
	case MISSINGVAR:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٱ���\n";
		break;
	case MISSINGFUNC:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٹؼ���function\n";
		break;
	case MISSSINGFUNCID:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٺ�����\n";
		break;
	case MISSINGLEFTBR:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ��������\'(\'\n";
		break;
	case MISSINGRIGHTBR:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ��������\')\'\n";
		break;
	case MISSINGARGUMENT:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٲ���\n";
		break;
	case UNDEDINEVAR:
		_ferr << "***LINE:" << _linecount << "  " << "���� " << _sym << " δ����\n";
		break;
	case MISSINGEUQ:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٵȺ�\'=\'\n";
		break;
	case MISSINGCOLON:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٸ�ֵ��\':=\'\n";
		break;
	case MISSINGIF:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٹؼ���if\n";
		break;
	case MISSINGTHEN:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٹؼ���then\n";
		break;
	case MISSINGELSE:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٹؼ���else\n";
		break;
	case MISSINGOPERATE:
		_ferr << "***LINE:" << _linecount << "  " << "ȱ�ٹ�ϵ�����\n";
		break;
	default:
		break;
	}
}

//��ʼ����
void RDParser::start()
{
	advance();
	S();
	PTGetValAdd();
	ShowValueTable();		//�����������
	ShowProcessTable();		//�����������

	//�ر��ļ�
	_fin.close();
	_ferr.close();
	_fvalt.close();
	_fprot.close();
}

//������������ݱ����������ƹ��������еı���λ����Ϣ
void RDParser::PTGetValAdd()
{
	int size = _ValueTable.size();
	for (int i = 0; i < size; i++) {
		std::string pid = _ValueTable[i]._vproc;
		if (_ProcessTable[pid]._fadr < 0) {
			_ProcessTable[pid]._fadr = _ValueTable[i]._vadr;
		}
		if (_ProcessTable[pid]._ladr < _ValueTable[i]._vadr) {
			_ProcessTable[pid]._ladr = _ValueTable[i]._vadr;
		}
	}
}

//�����������
void RDParser::ShowValueTable()
{
	_fvalt << "name" << '\t' << "address" << '\t' << "kind" << '\t' << "level" << '\t' << "process" << '\n';

	int size_1 = _ValueTable.size();
	for (int i = 0; i < size_1; i++) {
		
			std::string kind = (_ValueTable[i]._vkind == true) ? "�β�" : "ʵ��";
			_fvalt << _ValueTable[i]._vname << '\t' << _ValueTable[i]._vadr << '\t' << kind << '\t' << _ValueTable[i]._vlev << '\t' << _ValueTable[i]._vproc<<'\n';
	}
}

//�����������
void RDParser::ShowProcessTable()
{
	_fprot << "name" <<  '\t' << "level" << '\t'  << "faddress" << '\t' << "laddress" << '\n';

	std::map<std::string, Process>::iterator it;
	for (it = _ProcessTable.begin(); it != _ProcessTable.end(); it++) {
		_fprot << it->second._pname << '\t' << it->second._plev << '\t' << it->second._fadr << '\t' << it->second._ladr << '\n';

	}
}

//�������¶�ȡһ���ַ�����_sym��
void RDParser::advance()
{
	std::string line;
 	if (!_fin.eof()) {
		std::getline(_fin, line);
		line.erase(0, line.find_first_not_of(" "));		//ȥ���հ��ַ�

		//�ָ��ַ�������ȡ��Ԫʽ��Ϣ
		std::string::size_type pos, end;
		pos = line.find(" ");
		end = line.find("\n");
		_sym = line.substr(0, pos);
		_id = atoi(line.substr(pos + 1, end).c_str());

		//����������з�������+1���������¶�
		if (_id == 24 || _sym == "EOLN") {
			_linecount++;
			advance();
		}
	}
}

/*
�Ľ��ķ�G'���£�
S��begin A B end
A��C;A'
A'��C;A'|��
C��integer D
D��<��ʶ��> | function <��ʶ��>��<��ʶ��>����E
E��begin A B end
B��FB'
B'��;FB'|��
F��read(<��ʶ��>)��write(<��ʶ��>)��G��H
G��<��ʶ��>:=I
I��JI'
I'��-JI'|��
J��KJ'
J'��*KJ' | ��
K��<��ʶ��>��<����>��<��������>
H��if L then F else F
L��IMI
M��<��<=��>��>=��=��<>

���У�
S:<����>
A:<˵������>
A':<˵������>'
B:<ִ������>
B':<ִ������>'
C:<˵�����>
D:<��ʶ������˵��>
E:<������>
F:<ִ�����>
G:<��ֵ���>
H:<�������>
I:<�������ʽ>
I':<�������ʽ>'
J:<��>
J':<��>'
K:<����>
L:<�������ʽ>
M:<��ϵ�����>
*/

//<����>��begin <˵������><ִ������> end
//S��begin A B end
void RDParser::S()
{
	if (_sym != "begin") {
		error(MISSINGBEGIN);
	}
	advance();
	A();
	B();
	if (_sym == "end") {
		advance();
	}
	else {
		error(MISSINGEND);
	}
}

//<˵������>��<˵�����>��<˵������'>
//A��C;A'
void RDParser::A()
{
	C();
	if (_sym == ";") {
		advance();
		A_s();
	}
	else {
		error(MISSINGSEMICOLON);
	}
}

//<˵������'>��<˵�����>��<˵������'>|��
//A'��C;A'|��
void RDParser::A_s()
{
	C();
	if (_sym == ";") {
		advance();
		A_s(); 
	}
	else if (_sym == "if" || _sym == "read" || _sym == "write" || _id == CONSTANT) {
		//���';'����ִ����䣬������
	}
	else {
		error(MISSINGSEMICOLON);
	}
}

//<ִ������>��<ִ�����><ִ������'>
//B��FB'
void RDParser::B()
{
	F();
	B_s();
}

//<ִ������'>��; <ִ�����><ִ������'> | ��
//B'��;FB'|��
void RDParser::B_s()
{
	if (_sym == ";") {
		advance();
		F();
		B_s();
	}
	else if (_sym == "EOF" || _sym == "end") {
		//����ǻ��з�����һ�����̽�β������
	}
	else
	{
		error(MISSINGSEMICOLON);
	}
}

//<˵�����>��integer <��ʶ������˵��>
//C��integer D
void RDParser::C()
{
	if (_sym == "integer") {
		advance();
		D();
	}
	else if (_sym == "if" || _sym == "read" || _sym == "write" || _id == CONSTANT ) {
	
	}
	else {
		error(MISSINGTYPE);
		advance();
		D();
	}
}

//��ʶ������˵��>��<��ʶ��> | function <��ʶ��>��<��ʶ��>����<������>
//D��<��ʶ��> | function <��ʶ��>��<��ʶ��>����E
void RDParser::D()
 {
	if (_id == SYMBOL) {
		AddToValueTable(_sym, false);	
		advance();
	}
	else if (_sym == "function") {
		advance();
		if (_id == SYMBOL) {
			//����������ӵ����������У���ʱ���+1�����µ�ǰ����������������һ��������
			_level++;
			AddToProcessTable(_sym);
			_last_process_id = _current_process_id;
			_current_process_id = _sym;
			//��ԭ���
			_level--;
			advance();
			if (_sym == "(") {
				advance();
				if (_id == SYMBOL){
					_level++;
					AddToValueTable(_sym, true);	//���βα��浽����������
					_level--;
					advance();
					if (_sym == ")") {
						advance();
						if (_sym == ";") {
							advance();
							E();
							_level--;				//��ԭ���
							_current_process_id = _last_process_id;	//��ԭ��ǰ������
						}
						else error(MISSINGSEMICOLON);
					}
					else error(MISSINGRIGHTBR);
				}
				else error(MISSINGARGUMENT);
			}
			else error(MISSINGLEFTBR);
		}
		else error(MISSSINGFUNCID);
	}
	else error(MISSINGVAR);
}

//<������>��begin <˵������><ִ������> end
//E��begin A B end
void RDParser::E()
{
	if (_sym != "begin") {
		error(MISSINGBEGIN);
	}
	_level++;	//���+1
	advance();
	A();
	B();
	if (_sym == "end") {
		advance();
	}
	else {
		error(MISSINGEND);
	}
}

//<ִ�����>��read(<��ʶ��>)��write(<��ʶ��>)��<��ֵ���>��<�������>
//F��read(<��ʶ��>)��write(<��ʶ��>)��G��H
void RDParser::F()
{
	if (_sym == "read" || _sym == "write") {
		advance();
		if (_sym == "(") {
			advance();
			if (_id == SYMBOL) {
				if (!Is_in_ValueTable(_sym))
					error(UNDEDINEVAR);
				advance();
				if (_sym == ")") {
					advance();
				}
				else error(MISSINGRIGHTBR);
			}
			else error(MISSINGVAR);
		}
		else error(MISSINGLEFTBR);
	}
	else if (_id == SYMBOL) {
		G();
	}
	else if (_sym == "if") {
		H();
	}
}

//<��ֵ���>��<��ʶ��>:=<�������ʽ>
//G��<��ʶ��>:=I
void RDParser::G()
{
	if (_id == SYMBOL) {
		if (!Is_in_ValueTable(_sym) && !Is_in_ProcessTable(_sym))
			error(UNDEDINEVAR);
		advance();
		if (_sym == ":=") {
			advance();
			I();
		}
		else {
			error(MISSINGCOLON);
			advance();
			I();
		}
	}
	else error(MISSINGVAR);
}


//<�������>��if<�������ʽ>then<ִ�����>else <ִ�����>
//H��if L then F else F
void RDParser::H()
{
	if (_sym == "if") {
		advance();
		L();
		if (_sym == "then") {
			advance();
			F();
			if (_sym == "else") {
				advance();
				F();
			}
			else error(MISSINGELSE);
		}
		else error(MISSINGTHEN);
	}
	else error(MISSINGIF);
}

//<�������ʽ>��<��><�������ʽ>'
//I��JI'
void RDParser::I()
{
	J();
	I_s();
}

//<�������ʽ>'��-<��><�������ʽ>'
//I'��-JI'|��
void RDParser::I_s()
{
	if (_sym == "-") {
		advance();
		J();
		I_s();
	}
}

//<��>��<����><��'>
//J��KJ'
void RDParser::J()
{
	K();
	J_s();
}

//<��'>��*<����><��'> | ��
//J'��*KJ' | ��
void RDParser::J_s()
{
	if (_sym == "*") {
		advance();
		K();
		J_s();
	}
}

//<����>��<��ʶ��>��<����>��<��������>
//K��<��ʶ��>��<����>��<��������>
void RDParser::K()
{
	if (_id == SYMBOL) {
		if (!Is_in_ValueTable(_sym) && !Is_in_ProcessTable(_sym))
			error(UNDEDINEVAR);
		advance();
		if (_sym == "(") {
			advance();
			I();
			if (_sym == ")")
				advance();
			else
				error(MISSINGRIGHTBR);
		}
	}
	else if (_id == CONSTANT) {
		advance();
	}
}

//<�������ʽ>��<�������ʽ><��ϵ�����><�������ʽ>
//L��IMI
void RDParser::L()
{
	I();
	M();
	I();
}

//<��ϵ�����> ��<��<=��>��>=��=��<>
//M��<�� <= ��>�� >= �� = ��<>
void RDParser::M()
{
	if (_sym == "<" || _sym == "<=" || _sym == ">" || _sym == ">=" || _sym == "=" || _sym == "<>") {
		advance();
	}
	else error(MISSINGOPERATE);
}


