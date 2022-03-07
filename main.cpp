#include<iostream>
#include<fstream>
#include<stdio.h>
#include<conio.h>

#include<unordered_map>
#include<string>
#include<vector>

#include<windows.h>
#include <mmsystem.h>
#include<dsound.h>
#pragma comment(lib, "Winmm.lib")

using namespace std;

struct Student{
	string _id; //ѧ��
	string _name; //����
	int _chinese; //���ĳɼ�
	int _math; //��ѧ�ɼ�
	int _english; //Ӣ��ɼ�
	int _sum; //�ɼ��ܷ�
	int _level; //�ɼ�����
	Student * next; //������ĺ���ָ��
	Student * prev; //�������ǰ��ָ��

	Student(string id, string name, int chinese, int math,
	int english, int sum, int level, Student * n, Student * p):
		_id(id), _name(name), _chinese(chinese), _math(math), _english(english),
		_sum(sum), _level(level), next(n), prev(p){} //���캯��

	~Student(){} //��������
};

class List{ //˫������
	public:
		Student * head; //����ͷ
		Student * tail; //����β
		int length; //������
		unordered_map<string, Student *> dict; // ��ϣ��

		List(){ //���캯��
			this->head = new Student("null", "null", 0, 0, 0, 0, 0, nullptr, nullptr);
			this->tail = new Student("null", "null", 0, 0, 0, 0, 0, nullptr, nullptr);
			this->head->next = this->tail;
			this->tail->prev = this->head;
			this->length = 0;
		}

		~List(){ //��������
			delete head;
			delete tail;
		}

		void insert(Student * node){ //�����в���ڵ㣬ʹ�������������
			this->dict[node->_id] = node; //��ϣ���д洢ѧ�����㣬���������ӳ����Ϣ�����ڲ���
			this->dict[node->_name] = node;
			if(this->length == 0){ //�����ʱ����Ϊ��
				this->head->next = node;
				node->prev = this->head;
				this->tail->prev = node;
				node->next = this->tail;
				node->_level = 1;
			}
			else{
				Student * _left = this->head;//����һ����ָ���������
				while(_left->next != this->tail && node->_sum <= _left->next->_sum){
					//���Һ��ʵĲ����
					_left = _left->next;
				}
				Student * _right = _left->next;
				_left->next = node;//������
				node->prev = _left;
				_right->prev = node;
				node->next = _right;
				node->_level = (_left->_level) + 1;//�����²������ݵ�����
				while(_right != this->tail){//�޸ı��������ݺ��������������
					_right->_level++;
					_right = _right->next;
				}
			}
			this->length++;
		}

		void del(string str){ //����ѧ�Ż�����ɾ��ѧ������
			if(this->dict.find(str) == this->dict.end()){
				puts("���޴��ˣ������º˶�ѧ�Ż�������,�밴���������");
				return;
			}
			Student * node = this->dict[str];
			Student * _left = node->prev;
			Student * _right = node->next;
			_left->next = _right; //������ɾ������
			_right->prev = _left;
			dict.erase(node->_id); //��ϣ����ɾ������
			dict.erase(node->_name);
			delete node; //�ͷ��ڴ�
			while(_right != this->tail){//�޸ı��������ݺ��������������
				_right->_level--;
				_right = _right->next;
			}
			this->length--;
		}

		void modify(string str, string id, string name, int chinese, int math, int english){
			//����ѧ�Ż����������޸Ķ�������
			if(this->dict.find(str) == this->dict.end()){
				puts("���޴��ˣ������º˶�ѧ�Ż��������밴���������");
				return;
			}
			this->del(str);//ɾ��ԭ�ж���
			Student * node = new Student(id, name, chinese, math, english,
			chinese + math + english, 0, nullptr, nullptr);
			this->insert(node);//�����µĶ���
		}

		void select(string str){
			//����ѧ�Ż�������, ��ѯ����ӡ������Ϣ
			if(this->dict.find(str) == this->dict.end()){
				puts("���޴��ˣ������º˶�ѧ�Ż�������,�밴���������");
				return;
			}
			Student * node = this->dict[str];
			cout << "ѧ�ţ�" << node->_id << endl;
			cout << "������" << node->_name << endl;
			cout << "���ģ�" << node->_chinese << endl;
			cout << "��ѧ��" << node->_math << endl;
			cout << "Ӣ�" << node->_english << endl;
			cout << "�ܷ֣�" << node->_sum << endl;
			cout << "������" << node->_level << endl;
		}

		void show_all(void){
			//��ӡ���еĳɼ���Ϣ
			if(this->length == 0){
				puts("��ǰ���ݿ�Ϊ�գ��밴���������");
				return;
			}
			printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n","ѧ��", "����", "����", "��ѧ", "Ӣ��", "�ܷ�", "����");
			Student * p = this->head->next;
			while(p != this->tail){
				cout.setf(std::ios::left);
				cout.width(15);
				cout << p->_id;
				cout.width(15);
				cout << p->_name;
				printf("%-15d%-15d%-15d%-15d%-15d\n", p->_chinese, p->_math, p->_english, p->_sum, p->_level);

				p = p->next;
			}
		}
};

void showmenu(void);//��ʾ�˵�
void file_analy(const string& file_name, List * table);//���������ĵ����������ݽ���Ϊ����
void Stringsplit(const string& str, const char split, vector<string>& res);//ʵ���ַ����ָ�
void file_write(const string& file_name, List * table);//����������д����±�

void write_in(List * table);//¼��ɼ���ִ�й���1
void score_del(List * table);//ɾ���ɼ���ִ�й���2
void modify(List * table);//�޸���Ϣ��ִ�й���3
void select(List * table);//��ѯ��Ϣ��ִ�й���4

int main(void){
	int flag;
	string file_name = "file.txt";
	List * table = new List();
	file_analy(file_name, table);
	mciSendString(TEXT("open flower_dance.wma alias mysong"), NULL, 0, NULL);
	//��һ���Ǵ���Ƶ������������Ϊmysong.
	mciSendString(TEXT("play mysong repeat"), NULL, 0, NULL);//������
	//������Ƶ
	do{
		showmenu();
		scanf("%d", &flag);//ѡ�����
		switch (flag) {
			case 1:
				//����1��¼��ɼ�
				write_in(table);
				break;
			case 2:
				//����2��ɾ���ɼ�
				score_del(table);
				break;
			case 3:
				//����3���޸���Ϣ
				modify(table);
				break;
			case 4:
				//����4���޸���Ϣ
				select(table);
				break;
			case 5:
				//����5����ʾ���еĳɼ���Ϣ
				table->show_all();
				break;
			case 6:
				//����6��������Ϣ������
				file_write(file_name, table);
				break;
			case 0:
				//����0��������Ϣ���˳�ϵͳ
				goto end;
			default:
				//�Ƿ�����
				printf("������������ԣ�\n");
				getchar();
				break;
		}
		puts("�������������");
		getch();
	}while(1);

	end:
	file_write(file_name, table);
	mciSendString(TEXT("close mysong"), NULL, 0, NULL);//�ر�����
	puts("�ټ���");
	return 0;
}

void showmenu(void){//��ʾ�˵�
system("cls");
printf("*----------------------------------------------------*\n");
printf("* ��ӭʹ��**�ɼ�����ϵͳ *\n");
printf("*----------------------------------------------------*\n");
printf("* 1�� ¼��ɼ�*\n");
printf("* 2�� ɾ���ɼ�*\n");
printf("* 3�� �޸���Ϣ*\n");
printf("* 4�� �ɼ���ѯ*\n");
printf("* 5�� ��ʾ���гɼ���Ϣ*\n");
printf("* 6�� ������Ϣ������*\n");
printf("* 0�� ������Ϣ���˳�ϵͳ*\n");
printf("*----------------------------------------------------*\n");
printf("ѡ�����<0-6> \n");
}

void file_analy(const string& file_name, List * table){//���������ĵ����������ݽ���Ϊ����
	ifstream in(file_name);//�ļ�����
	string s;
	vector<string> arr;//��̬���飬����ָ����ַ���
	string id, name;
	int chinese, math, english, sum, level;
	while(getline(in,s)){//���ж�ȡ�ĵ�����
		Stringsplit(s, ' ', arr);//�ַ����ָ�
		id = arr[0];
		name = arr[1];
		chinese = stoi(arr[2]);
		math = stoi(arr[3]);
		english = stoi(arr[4]);
		sum = stoi(arr[5]);
		level = stoi(arr[6]);
		Student * node = new Student(id, name, chinese, math, english, sum, level, nullptr, nullptr);
		table->insert(node);//�������������
		arr.clear();
	}
}

void Stringsplit(const string& str, const char split, vector<string>& res){
	//ʵ���ַ����ָ�,Ҳ��һ�ַ����㷨��Ӧ�ó���
	if (str == "")		return;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	string strs = str + split;
	size_t pos = strs.find(split);

	// ���Ҳ����������ַ��������������� npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
	}
}

void file_write(const string& file_name, List * table){//����������д����±�
	puts("��Ϣ������...");
	ofstream file_writer(file_name, ios_base::out);
	Student * p = table->head->next;
	while(p != table->tail){
		file_writer << p->_id << " " << p->_name << " ";
		file_writer << p->_chinese << " " << p->_math << " ";
		file_writer << p->_english << " " << p->_sum << " ";
		file_writer << p->_level << endl;

		p = p->next;
	}
	file_writer.close();
	puts("����ɹ���");
}

void write_in(List * table){//¼��ɼ���ִ�й���1
	int n, chinese, math, english;
	string id, name;
	printf("��������Ҫ¼���������");
	scanf("%d", &n);
	printf("������ȫ��ѧ����ѧ�ţ����������ĳɼ�����ѧ�ɼ���Ӣ��ɼ���\n");
	for(int i = 0; i < n; ++i){
		printf("----��%d��ѧ��----\n", i+1);

		printf("ѧ��:");
		cin >> id;

		printf("������");
		cin >> name;

		printf("���ĳɼ���");
		scanf("%d", &chinese);

		printf("��ѧ�ɼ���");
		scanf("%d", &math);

		printf("Ӣ��ɼ���");
		scanf("%d", &english);

		Student * node = new Student(id, name, chinese, math, english,
					chinese + math + english, 0, nullptr, nullptr);//��������

		table->insert(node);//�������������
	}
}

void score_del(List * table){//ɾ���ɼ���ִ�й���2
	string str;
	printf("��������Ҫɾ����ѧ��ѧ�Ż�����:");
	cin >> str;
	table->del(str);
}

void modify(List * table){//�޸���Ϣ��ִ�й���3
	string str, id, name;
	int chinese, math, english;
	printf("��������Ҫɾ����ѧ��ѧ�Ż�����:");
	cin >> str;

	printf("�������ѧ���޸ĺ��ѧ�ţ�");
	cin >> id;

	printf("�������ѧ���޸ĺ��������");
	cin >> name;

	printf("�������ѧ���޸ĺ�����ĳɼ���");
	scanf("%d", &chinese);

	printf("�������ѧ���޸ĺ����ѧ�ɼ���");
	scanf("%d", &math);

	printf("�������ѧ���޸ĺ��Ӣ��ɼ���");
	scanf("%d", &english);

	table->modify(str, id, name, chinese, math, english);
}

void select(List * table){//��ѯ��Ϣ��ִ�й���4
	string str;
	printf("��������Ҫ��ѯ��ѧ��ѧ�Ż�����:");
	cin >> str;
	table->select(str);
}
